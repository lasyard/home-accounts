#ifndef _DATA_ROW_DAO_H_
#define _DATA_ROW_DAO_H_

#include <string>
#include <vector>

#include "CsvDao.h"
#include "csv/CsvExceptions.h"
#include "csv/money.h"
#include "csv/str.h"

/**
 * @brief Dao to read csv into a vector.
 *
 * @tparam I the element type of row
 * @tparam AUTO the column which is incremented automatically
 */
template <typename I, int AUTO> class CsvVecDao : public CsvDao<I, std::vector<I>>
{
    typedef CsvRowTraits<I> Traits;
    typedef std::vector<I> T;
    typedef CsvDao<I, T> Csv;

public:
    CsvVecDao() : Csv()
    {
    }

    virtual ~CsvVecDao()
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

    int getNumberRows() const override
    {
        return Dao<T>::m_data.size();
    }

    std::string getRowLabel(int row)
    {
        return std::to_string(row);
    }

    std::string getString(int row, int col)
    {
        auto &data = Dao<T>::m_data;
        return Csv::m_parser->toStringOfColumn(col, Traits::writePtr(&data[row], col));
    }

    void setString(int row, int col, const std::string &value)
    {
        auto &data = Dao<T>::m_data;
        Csv::m_parser->parseStringOfColumn(value, col, Traits::readPtr(&data[row], col));
    }

    bool insert(size_t pos)
    {
        auto &data = Dao<T>::m_data;
        I item;
        initItem(&item);
        data.insert(std::next(data.begin(), pos), std::move(item));
        return true;
    }

    bool append()
    {
        auto &data = Dao<T>::m_data;
        I item;
        initItem(&item);
        data.push_back(std::move(item));
        return true;
    }

    bool isAutoIncrement(int i) const
    {
        return i == AUTO;
    }

private:
    void initItem(I *item)
    {
        for (int i = 0; i < Traits::cols; ++i) {
            switch (Traits::types[i]) {
            case INT32:
                *(int32_t *)Traits::readPtr(item, i) = isAutoIncrement(i) ? nextValue<int32_t>(i) : 0;
                break;
            case INT64:
                *(int64_t *)Traits::readPtr(item, i) = isAutoIncrement(i) ? nextValue<int64_t>(i) : 0;
                break;
            case MONEY:
                *(money_t *)Traits::readPtr(item, i) = 0;
                break;
            case CSTR:
                // Important, or it will be freed.
                *(char **)Traits::readPtr(item, i) = nullptr;
                break;
            default:
                break;
            }
        }
    }

    template <typename D> D nextValue(int i)
    {
        auto &data = Dao<T>::m_data;
        D m = (D)0;
        for (auto d : data) {
            D v = *(D *)Traits::writePtr(&d, i);
            if (v > m) {
                m = v;
            }
        }
        return m + 1;
    }
};

#endif /* _DATA_ROW_DAO_H_ */
