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
    using D = Dao<std::vector<I>>;
    using CD = CsvDao<I, std::vector<I>>;
    using CVD = CsvVecDao<I>;

    using ID_TYPE = ColType<I, ID_COL>;

public:
    CsvIdVecDao(const std::string &name = "") : CVD(name)
    {
    }

    virtual ~CsvIdVecDao()
    {
    }

protected:
    void initItemField(I *item, int i) const override
    {
        if (i == ID_COL) {
            *(ID_TYPE *)CD::Traits::getPtr(item, i) = nextValue();
        } else {
            CVD::initItemField(item, i);
        }
    }

private:
    ID_TYPE nextValue() const
    {
        auto &data = D::m_data;
        ID_TYPE m = (ID_TYPE)0;
        for (auto d : data) {
            ID_TYPE v = *(ID_TYPE *)CD::Traits::getPtr(&d, ID_COL);
            if (v > m) {
                m = v;
            }
        }
        return m + 1;
    }
};

#endif /* _DATA_CSV_ID_VEC_DAO_H_ */
