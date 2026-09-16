#ifndef _HA_DATA_YEARS_DOC_H_
#define _HA_DATA_YEARS_DOC_H_

#include "../HaCsv.h"

#include "csv/date_time.h"
#include "csv/str.h"

class DataDoc;

class YearsDoc : public HaCsv
{
public:
    static constexpr int YEAR_COL = 0;
    static constexpr int INCOME_COL = 1;
    static constexpr int OUTLAY_COL = 2;
    static constexpr int RECORDS_COL = 3;
    static constexpr int COLS = 4;

    YearsDoc();
    virtual ~YearsDoc();

    year_t GetRecordYear(const record_t *record) const
    {
        return *(year_t *)get_const_field(&m_parser, record, YEAR_COL);
    }

    money_t GetRecordIncome(const record_t *record) const
    {
        return *(money_t *)get_const_field(&m_parser, record, INCOME_COL);
    }

    money_t GetRecordOutlay(const record_t *record) const
    {
        return *(money_t *)get_const_field(&m_parser, record, OUTLAY_COL);
    }

    void SetSummary(const DataDoc *data);

    money_t CalcBalance(year_t year) const;

private:
    static constexpr const column_type COL_TYPES[] = {
        CT_YEAR,
        CT_MONEY,
        CT_MONEY,
        CT_INT,
    };

    static constexpr const str COL_TITLES[] = {
        {   wxTRANSLATE("Year"), 4, false},
        { wxTRANSLATE("Income"), 6, false},
        { wxTRANSLATE("Outlay"), 6, false},
        {wxTRANSLATE("Records"), 7, false},
    };
};

#endif /* _HA_DATA_YEARS_DOC_H_ */
