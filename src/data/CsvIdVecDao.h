#ifndef _DATA_CSV_ID_VEC_DAO_H_
#define _DATA_CSV_ID_VEC_DAO_H_

#include "CsvVecDao.h"

/**
 * @brief Dao to read csv into a vector, with an unique ID column.
 *
 * @tparam I the element type of row
 * @tparam ID_COL the column which is incremented automatically
 */
template <typename I, int ID_COL = 0> class CsvIdVecDao : public CsvVecDao<I>
{
    typedef CsvRowTraits<I> Traits;
    typedef std::vector<I> T;
    typedef CsvDao<I, T> Csv;

    template <int COL> using ColType = typename TypeGetter<Traits::types[COL]>::Type;

    typedef ColType<ID_COL> ID_TYPE;

public:
    CsvIdVecDao() : CsvVecDao<I>()
    {
    }

    virtual ~CsvIdVecDao()
    {
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
