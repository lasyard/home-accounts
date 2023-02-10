#ifndef _DATA_ROW_DAO_H_
#define _DATA_ROW_DAO_H_

#include <vector>

#include "CsvDao.h"
#include "csv/CsvExceptions.h"
#include "csv/str.h"

template <typename I> class RowDao : public CsvDao<I, std::vector<I>>
{
    typedef CsvRowTraits<I> Traits;
    typedef std::vector<I> T;
    typedef CsvDao<I, T> Csv;

public:
    RowDao() : Csv()
    {
    }

    virtual ~RowDao()
    {
        for (auto &item : Dao<T>::m_data) {
            for (int i = 0; i < Traits::cols; ++i) {
                if (Traits::types[i] == CSTR) {
                    // Important
                    char *p = (char *)Traits::writePtr(&item, i);
                    if (p != nullptr) {
                        free(p);
                    }
                }
            }
        }
    }

    void read(std::istream &is) override
    {
        auto &data = Dao<T>::m_data;
        auto &buf = Csv::m_buf;
        int lineNo = 0;
        data.clear();
        while (is.getline(buf, Csv::MAX_LINE_LENGTH)) {
            lineNo++;
            if (is_line_end(buf[0])) {
                continue;
            }
            try {
                I item;
                initItem(&item);
                Csv::m_parser->parseLine(buf, &item);
                data.push_back(std::move(item));
            } catch (ParseError &e) {
                e.setLineNo(lineNo);
                throw;
            }
        }
    }

    void write(std::ostream &os) override
    {
        auto &data = Dao<T>::m_data;
        auto &buf = Csv::m_buf;
        for (auto item : data) {
            Csv::m_parser->outputLine(buf, &item);
            os << buf << std::endl;
        }
    }

private:
    void initItem(I *item)
    {
        for (int i = 0; i < Traits::cols; ++i) {
            if (Traits::types[i] == CSTR) {
                // Important, or it will be freed.
                *(char **)Traits::readPtr(item, i) = nullptr;
            }
        }
    }
};

#endif /* _DATA_ROW_DAO_H_ */
