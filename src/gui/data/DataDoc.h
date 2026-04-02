#ifndef _HA_DATA_DATA_DOC_H_
#define _HA_DATA_DATA_DOC_H_

#include <map>

#include "../HaCsvTemplate.h"

#include "../BiMap.h"

#include "csv/date_time.h"
#include "csv/money.h"

class DataDoc : public HaCsvTemplate<DataDoc>
{
public:
    DECLARE_TM(DataDoc)

    static constexpr int COLS = 9;
    static constexpr int DATE_COL = 0;
    static constexpr int TIME_COL = 1;
    static constexpr int AMOUNT_COL = 2;
    static constexpr int ACCOUNT_COL = 3;
    static constexpr int DESC_COL = 4;
    static constexpr int REAL_AMOUNT_COL = 5;
    static constexpr int REAL_DESC_COL = 6;
    static constexpr int MEMO_COL = 7;
    static constexpr int AUTO_SET_COL = 8;

    static constexpr int DATETIME_VIRTUAL_COL = -2;

    DataDoc(int year);
    virtual ~DataDoc();

    static const column_type COL_TYPES[COLS];

    static wxString GetColName(int i);
    static int GetColByName(const wxString &name);
    static void GetAllColNames(wxArrayString &colNames);

    void SetAccountIdAndNames(const std::vector<int64_t> &ids, const wxArrayString &names);

    const wxArrayString &GetAccountNames() const
    {
        return m_accountNames;
    }

    money_t GetRecordRealAmount(const record_t *record) const
    {
        wxASSERT(record->flag == RECORD_FLAG_NORMAL);
        return *(money_t *)get_const_field(&m_parser, record, REAL_AMOUNT_COL);
    }

    date_t GetRecordDate(const record_t *record) const
    {
        return *(date_t *)get_const_field(&m_parser, record, DATE_COL);
    }

    timo_t GetRecordTime(const record_t *record) const
    {
        return *(timo_t *)get_const_field(&m_parser, record, TIME_COL);
    }

    money_t GetRecordBalance(const record_t *record) const
    {
        if (record->udata != NULL) {
            return static_cast<ExtraCols *>(record->udata)->balance;
        }
        return 0;
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

    void SetRecordBalance(record_t *record, money_t balance) const
    {
        if (record->udata == NULL) {
            record->udata = malloc(sizeof(ExtraCols));
            if (record->udata == NULL) {
                return;
            }
        }
        static_cast<ExtraCols *>(record->udata)->balance = balance;
    }

    const wxString GetIncomeString(int row) const
    {
        auto *record = GetRecord(row);
        wxASSERT(record != nullptr);
        money_t m = GetRecordRealAmount(record);
        if (m < 0) {
            return GetMoneyString(-m);
        }
        return wxEmptyString;
    }

    const wxString GetOutlayString(int row) const
    {
        auto *record = GetRecord(row);
        wxASSERT(record != nullptr);
        money_t m = GetRecordRealAmount(record);
        if (m > 0) {
            return GetMoneyString(m);
        }
        return wxEmptyString;
    }

    const wxString GetBalanceString(int row) const
    {
        auto *record = GetRecord(row);
        wxASSERT(record != nullptr);
        money_t balance = GetRecordBalance(record);
        return GetMoneyString(balance);
    }

    void SetOpening(money_t opening);
    void UpdateBalanceStat();

    int FindDateRow(int year, int month, int day);

    record_t *InsertRecordAtTime(date_t date, timo_t time);

    wxString GetStatOpeningString() const
    {
        return GetMoneyString(m_stat.opening);
    }

    wxString GetStatClosingString() const
    {
        return GetMoneyString(m_stat.closing);
    }

    wxString GetStatIncomeString() const
    {
        return GetMoneyString(m_stat.income);
    }

    wxString GetStatOutlayString() const
    {
        return GetMoneyString(m_stat.outlay);
    }

protected:
    const wxString AccountGetter(const record_t *record, int i) const;
    void AccountSetter(record_t *record, int i, const wxString &value);

    bool AfterRead() override;

    bool IsRecordEmpty(record_t *record) override;

private:
    struct ExtraCols {
        money_t balance;
    };

    struct Stat {
        money_t opening;
        money_t closing;
        money_t income;
        money_t outlay;
    } m_stat;

    int m_year;

    wxArrayString m_accountNames;
    BiMap<int64_t, wxString> m_accountIdNameMap;
};

#endif /* _HA_DATA_DATA_DOC_H_ */
