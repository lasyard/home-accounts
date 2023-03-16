#ifndef _DATA_CSV_ID_VEC_DAO_H_
#define _DATA_CSV_ID_VEC_DAO_H_

#include "CsvVecDao.h"

/**
 * @brief Dao to read csv into a vector, with an unique ID column.
 *
 * @tparam I the element type of row
 * @tparam ID_TYPE the type of ID column
 * @tparam ID_COL the column which is incremented automatically
 */
template <typename I, typename ID_TYPE, int ID_COL = 0> class CsvIdVecDao : public CsvVecDao<I>
{
    typedef CsvRowTraits<I> Traits;
    typedef std::vector<I> T;

public:
    CsvIdVecDao() : CsvVecDao<I>()
    {
    }

    virtual ~CsvIdVecDao()
    {
    }

    const I *getById(ID_TYPE id)
    {
        const auto &data = Dao<T>::m_data;
        for (const auto &item : data) {
            if (*(ID_TYPE *)Traits::writePtr(&item, ID_COL) == id) {
                return &item;
            }
        }
        return nullptr;
    }

    /**
     * @brief Get the name by id. The `name` column must exits and have type `CSTR`.
     *
     * @param id the id
     * @return the name
     */
    template <int NAME_COL> const char *getNameById(ID_TYPE id)
    {
        auto item = getById(id);
        return (item != nullptr) ? (const char *)Traits::writePtr(item, NAME_COL) : nullptr;
    }

    template <int NAME_COL> ID_TYPE getIdByName(const char *name)
    {
        if (name != nullptr) {
            const auto &data = Dao<T>::m_data;
            for (const auto &item : data) {
                auto n = (const char *)Traits::writePtr(&item, NAME_COL);
                if (n != nullptr && strcmp(n, name) == 0) {
                    return *(ID_TYPE *)Traits::writePtr(&item, ID_COL);
                }
            }
        }
        return (ID_TYPE)0;
    }

protected:
    void initItemField(I *item, int i) const override
    {
        if (i == ID_COL) {
            *(ID_TYPE *)Traits::readPtr(item, i) = nextValue();
        } else {
            CsvVecDao<I>::initItemField(item, i);
        }
    }

private:
    ID_TYPE nextValue() const
    {
        auto &data = Dao<T>::m_data;
        ID_TYPE m = (ID_TYPE)0;
        for (auto d : data) {
            ID_TYPE v = *(ID_TYPE *)Traits::writePtr(&d, ID_COL);
            if (v > m) {
                m = v;
            }
        }
        return m + 1;
    }
};

#endif /* _DATA_CSV_ID_VEC_DAO_H_ */
