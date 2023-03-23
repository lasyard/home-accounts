#ifndef _DATA_CSV_ID_VEC_DAO_H_
#define _DATA_CSV_ID_VEC_DAO_H_

#include "CsvVecDao.h"
#include "TypeGetter.h"

/**
 * @brief Dao to read csv into a vector, with an unique ID column.
 *
 * @tparam I the element type of row
 * @tparam ID_COL the column which is incremented automatically
 */
template <typename I, int ID_COL = 0> class CsvIdVecDao : public CsvVecDao<I>
{
    typedef CsvRowTraits<I> Traits;

    template <int COL> using TG = TypeGetter<Traits::types[COL]>;
    template <int COL> using TGV = typename TG<COL>::ValueType;

    typedef std::vector<I> T;
    typedef TGV<ID_COL> ID_TYPE;

public:
    CsvIdVecDao() : CsvVecDao<I>()
    {
    }

    virtual ~CsvIdVecDao()
    {
    }

    I *getById(ID_TYPE id)
    {
        auto &data = Dao<T>::m_data;
        for (auto &item : data) {
            if (*(ID_TYPE *)Traits::getPtr(&item, ID_COL) == id) {
                return &item;
            }
        }
        return nullptr;
    }

    /**
     * @brief Get value of another field by id.
     *
     * @param id the id
     * @return the value
     */
    template <int V_COL> TGV<V_COL> getValueById(ID_TYPE id)
    {
        auto item = getById(id);
        return (item != nullptr) ? *(TGV<V_COL> *)Traits::getPtr(item, V_COL) : TG<V_COL>::zero();
    }

    template <int V_COL> ID_TYPE getIdByValue(TGV<V_COL> value)
    {
        if (value != TG<V_COL>::zero()) {
            auto &data = Dao<T>::m_data;
            for (auto &item : data) {
                auto v = *(TGV<V_COL> *)Traits::getPtr(&item, V_COL);
                if (v != TG<V_COL>::zero() && TG<V_COL>::compare(v, value) == 0) {
                    return *(ID_TYPE *)Traits::getPtr(&item, ID_COL);
                }
            }
        }
        return TG<ID_COL>::zero();
    }

protected:
    void initItemField(I *item, int i) const override
    {
        if (i == ID_COL) {
            *(ID_TYPE *)Traits::getPtr(item, i) = nextValue();
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
            ID_TYPE v = *(ID_TYPE *)Traits::getPtr(&d, ID_COL);
            if (v > m) {
                m = v;
            }
        }
        return m + 1;
    }
};

#endif /* _DATA_CSV_ID_VEC_DAO_H_ */
