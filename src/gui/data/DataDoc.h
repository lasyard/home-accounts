#ifndef _HA_DATA_DATA_DOC_H_
#define _HA_DATA_DATA_DOC_H_

#include "../CsvDoc.h"

#include "data/data.h"

class DataDoc : public CsvDoc
{
public:
    DECLARE_TM()

    DataDoc(int year, int month);
    virtual ~DataDoc();

    const struct data_stat *GetStat() const
    {
        return &m_stat;
    }

    void SetOpening(money_t opening);
    void UpdateBalanceStat();

private:
    int m_year;
    int m_month;
    struct data_stat m_stat;

    bool AfterRead() override;
    bool BeforeWrite() override;
};

#endif /* _HA_DATA_DATA_DOC_H_ */
