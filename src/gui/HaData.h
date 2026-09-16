#ifndef _HA_GUI_HA_DATA_H_
#define _HA_GUI_HA_DATA_H_

#include "HaCsv.h"

#include "csv/date_time.h"
#include "csv/money.h"
#include "csv/str.h"

class HaData : public HaCsv
{
public:
    DECLARE_TM(HaData)

    HaData();
    virtual ~HaData();

    static constexpr const int DATE_COL = 0;
    static constexpr const int TIME_COL = 1;
    static constexpr const int ACCOUNT_COL = 2;
    static constexpr const int AMOUNT_COL = 3;
    static constexpr const int DESC_COL = 4;
    static constexpr const int MEMO_COL = 5;
    static constexpr const int COLS = 6;

    static constexpr const column_type COL_TYPES[COLS] = {
        CT_DATE,
        CT_TIME,
        CT_INT,
        CT_MONEY,
        CT_STR,
        CT_STR,
    };

    static constexpr const str COL_TITLES[COLS] = {
        {   wxTRANSLATE("Date"), 4, false},
        {   wxTRANSLATE("Time"), 4, false},
        {wxTRANSLATE("Account"), 7, false},
        { wxTRANSLATE("Amount"), 6, false},
        {   wxTRANSLATE("Desc"), 4, false},
        {   wxTRANSLATE("Memo"), 4, false},
    };

    money_t GetRecordAmount(const record_t *record) const
    {
        wxASSERT(record->flag == RECORD_FLAG_NORMAL);
        return *(money_t *)get_const_field(&m_parser, record, AMOUNT_COL);
    }

    date_t GetRecordDate(const record_t *record) const
    {
        return *(date_t *)get_const_field(&m_parser, record, DATE_COL);
    }

    timo_t GetRecordTime(const record_t *record) const
    {
        return *(timo_t *)get_const_field(&m_parser, record, TIME_COL);
    }

    void SetRecordDate(record_t *record, date_t date) const
    {
        *(date_t *)get_field(&m_parser, record, DATE_COL) = date;
    }

    void SetRecordTime(record_t *record, timo_t time) const
    {
        *(timo_t *)get_field(&m_parser, record, TIME_COL) = time;
    }

    void SetRecordAccount(record_t *record, int64_t account) const
    {
        *(int64_t *)get_field(&m_parser, record, ACCOUNT_COL) = account;
    }

    const wxString GetIncomeString(int row) const
    {
        auto *record = GetRecord(row);
        wxASSERT(record != nullptr);
        money_t m = GetRecordAmount(record);
        if (m < 0) {
            return GetMoneyString(-m);
        }
        return wxEmptyString;
    }

    const wxString GetOutlayString(int row) const
    {
        auto *record = GetRecord(row);
        wxASSERT(record != nullptr);
        money_t m = GetRecordAmount(record);
        if (m > 0) {
            return GetMoneyString(m);
        }
        return wxEmptyString;
    }

protected:
    bool IsRecordEmpty(record_t *record) override;
};

#endif /* _HA_GUI_HA_DATA_H_ */
