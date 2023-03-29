#ifndef _DATA_CSV_VEC_DAO_H_
#define _DATA_CSV_VEC_DAO_H_

#include <string>
#include <vector>

#include "CsvDao.h"
#include "Joint.h"
#include "TypeGetter.h"
#include "TypeTraits.h"
#include "csv/CsvExceptions.h"
#include "csv/money.h"
#include "csv/str.h"

/**
 * @brief Dao to read csv into a vector.
 *
 * @tparam I the element type of row
 */
template <typename I> class CsvVecDao : public CsvDao<I, std::vector<I>>
{
    typedef CsvRowTraits<I> Traits;
    typedef std::vector<I> T;
    typedef CsvDao<I, T> Csv;

    template <int COL> using ColType = typename TypeGetter<Traits::types[COL]>::Type;
    template <int COL> using ColTypeTraits = TypeTraits<ColType<COL>>;

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
                    char *p = *(char **)Traits::getPtr(&item, i);
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
        return Csv::m_parser->toStringOfColumn(col, Traits::getPtr(&data[row], col));
    }

    void setString(int row, int col, const std::string &value)
    {
        auto &data = Dao<T>::m_data;
        Csv::m_parser->parseStringOfColumn(value, col, Traits::getPtr(&data[row], col));
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

    bool remove(size_t pos)
    {
        auto &data = Dao<T>::m_data;
        data.erase(std::next(data.begin(), pos));
        return true;
    }

    template <int COL> I *getByCol(ColType<COL> v)
    {
        auto &data = Dao<T>::m_data;
        for (auto &item : data) {
            if (ColTypeTraits<COL>::compare(*(ColType<COL> *)Traits::getPtr(&item, COL), v) == 0) {
                return &item;
            }
        }
        return nullptr;
    }

    /**
     * @brief Get the value of a col.
     *
     * @tparam D_COL destination col
     * @tparam S_COL source col
     * @param id value of source col
     * @return ColType<D_COL> value of destination col
     */
    template <int D_COL, int S_COL> ColType<D_COL> getColByCol(ColType<S_COL> v)
    {
        auto item = getByCol<S_COL>(v);
        return (item != nullptr) ? *(ColType<D_COL> *)Traits::getPtr(item, D_COL) : ColTypeTraits<D_COL>::zero();
    }

    /**
     * @brief Get the Joint object of this dao.
     *
     * @tparam D_COL index of target column
     * @tparam S_COL index of source column
     * @return Joint  the object
     */
    template <int D_COL, int S_COL> Joint<ColType<D_COL>, ColType<S_COL>> getJoint()
    {
        return Joint<ColType<D_COL>, ColType<S_COL>>(
            [this](ColType<S_COL> v) -> auto{ return this->getColByCol<D_COL, S_COL>(v); },
            [this](ColType<D_COL> v) -> auto{ return this->getColByCol<S_COL, D_COL>(v); }
        );
    }

protected:
    virtual void initItemField(I *item, int i) const
    {
        switch (Traits::types[i]) {
        case INT32:
            *(int32_t *)Traits::getPtr(item, i) = 0;
            break;
        case INT64:
            *(int64_t *)Traits::getPtr(item, i) = 0;
            break;
        case MONEY:
            *(money_t *)Traits::getPtr(item, i) = 0;
            break;
        case CSTR:
            // Important, or it will be freed.
            *(const char **)Traits::getPtr(item, i) = nullptr;
            break;
        default:
            break;
        }
    }

private:
    void initItem(I *item) const
    {
        for (int i = 0; i < Traits::cols; ++i) {
            initItemField(item, i);
        }
    }
};

#endif /* _DATA_CSV_VEC_DAO_H_ */
