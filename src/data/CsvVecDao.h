#ifndef _DATA_CSV_VEC_DAO_H_
#define _DATA_CSV_VEC_DAO_H_

#include <string>
#include <vector>

#include "CsvDao.h"
#include "Joint.h"
#include "TypeGetter.h"

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
    using D = Dao<std::vector<I>>;
    using CD = CsvDao<I, std::vector<I>>;

public:
    CsvVecDao(const std::string &name = "") : CD(name)
    {
    }

    virtual ~CsvVecDao()
    {
        for (auto &item : D::m_data) {
            for (int i = 0; i < CD::Traits::cols; ++i) {
                if (CD::Traits::types[i] == CSTR) {
                    // Important
                    char *p = *(char **)CD::Traits::getPtr(&item, i);
                    if (p != nullptr) {
                        free(p);
                    }
                }
            }
        }
    }

    template <int T_COL, int S_COL> class VecJoint : public Joint<ColType<I, T_COL>, ColType<I, S_COL>>
    {
    public:
        VecJoint(CsvVecDao<I> *dao) : m_dao(dao)
        {
        }

        VecJoint(const VecJoint &obj) : m_dao(obj.m_dao)
        {
        }

        virtual ~VecJoint()
        {
        }

        const VecJoint &operator=(const VecJoint &obj)
        {
            m_dao = obj.m_dao;
            return *this;
        }

        ColType<I, T_COL> lookup(ColType<I, S_COL> s) const override
        {
            return m_dao->getColByCol<T_COL, S_COL>(s);
        }

        ColType<I, S_COL> revLookup(ColType<I, T_COL> d) const override
        {
            return m_dao->getColByCol<S_COL, T_COL>(d);
        }

        void forEachTarget(std::function<bool(const ColType<I, T_COL> *)> callback) const override
        {
            m_dao->forEach<T_COL>(callback);
        }

        void forEachSource(std::function<bool(const ColType<I, S_COL> *)> callback) const override
        {
            m_dao->forEach<S_COL>(callback);
        }

    private:
        CsvVecDao<I> *m_dao;
    };

    void read(std::istream &is) override
    {
        auto &data = D::m_data;
        auto &buf = CD::m_buf;
        int lineNo = 0;
        data.clear();
        while (is.getline(buf, CD::MAX_LINE_LENGTH)) {
            lineNo++;
            if (is_line_end(buf[0])) {
                continue;
            }
            try {
                I item;
                initItem(&item);
                CD::m_parser->parseLine(buf, &item);
                data.push_back(std::move(item));
            } catch (ParseError &e) {
                e.setLineNo(lineNo);
                throw;
            }
        }
    }

    void write(std::ostream &os) const override
    {
        auto &data = D::m_data;
        auto &buf = CD::m_buf;
        for (auto item : data) {
            CD::m_parser->outputLine(buf, &item);
            os << buf << std::endl;
        }
    }

    int getNumberRows() const override
    {
        return D::m_data.size();
    }

    std::string getRowLabel(int row)
    {
        return std::to_string(row);
    }

    virtual std::string getString(int row, int col)
    {
        auto &data = D::m_data;
        return CD::m_parser->toStringOfColumn(col, CD::Traits::getPtr(&data[row], col));
    }

    virtual void setString(int row, int col, const std::string &value)
    {
        auto &data = D::m_data;
        CD::m_parser->parseStringOfColumn(value, col, CD::Traits::getPtr(&data[row], col));
    }

    I *insert(size_t pos)
    {
        auto &data = D::m_data;
        I item;
        initItem(&item);
        auto i = data.insert(std::next(data.begin(), pos), std::move(item));
        return &*i;
    }

    void append()
    {
        auto &data = D::m_data;
        I item;
        initItem(&item);
        data.push_back(std::move(item));
    }

    I *last()
    {
        auto &data = D::m_data;
        return &data.back();
    }

    void remove(size_t pos)
    {
        auto &data = D::m_data;
        data.erase(std::next(data.begin(), pos));
    }

    template <int COL> I *getByCol(ColType<I, COL> v)
    {
        auto &data = D::m_data;
        for (auto &item : data) {
            if (ColTraits<I, COL>::compare(*(ColType<I, COL> *)CD::Traits::getPtr(&item, COL), v) == 0) {
                return &item;
            }
        }
        return nullptr;
    }

    /**
     * @brief Get the value of a col.
     *
     * @tparam T_COL target col
     * @tparam S_COL source col
     * @param v value of source col
     * @return value of target col
     */
    template <int T_COL, int S_COL> ColType<I, T_COL> getColByCol(ColType<I, S_COL> v)
    {
        auto item = getByCol<S_COL>(v);
        return (item != nullptr) ? *(ColType<I, T_COL> *)CD::Traits::getPtr(item, T_COL) : ColTraits<I, T_COL>::zero();
    }

    /**
     * @brief Get the Joint object of this dao.
     *
     * @tparam T_COL index of target column
     * @tparam S_COL index of source column
     * @return pointer to the object, released by the caller
     */
    template <int T_COL, int S_COL> auto *getJoint()
    {
        return new VecJoint<T_COL, S_COL>(this);
    }

    template <int COL> void forEach(std::function<bool(const ColType<I, COL> *)> callback)
    {
        auto &data = D::m_data;
        for (auto &item : data) {
            if (!callback((const ColType<I, COL> *)CD::Traits::getPtr(&item, COL))) {
                break;
            }
        }
    }

protected:
    virtual void initItemField(I *item, int i) const
    {
        switch (CD::Traits::types[i]) {
        case INT32:
            *(int32_t *)CD::Traits::getPtr(item, i) = 0;
            break;
        case INT64:
            *(int64_t *)CD::Traits::getPtr(item, i) = 0;
            break;
        case MONEY:
            *(money_t *)CD::Traits::getPtr(item, i) = 0;
            break;
        case CSTR:
            // Important, or it will be freed.
            *(const char **)CD::Traits::getPtr(item, i) = nullptr;
            break;
        default:
            break;
        }
    }

private:
    void initItem(I *item) const
    {
        for (int i = 0; i < CD::Traits::cols; ++i) {
            initItemField(item, i);
        }
    }
};

#endif /* _DATA_CSV_VEC_DAO_H_ */
