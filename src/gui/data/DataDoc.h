#ifndef _HA_DATA_DATA_DOC_H_
#define _HA_DATA_DATA_DOC_H_

#include "../HaData.h"

#include "../BiMap.h"

#include "csv/date_time.h"
#include "csv/money.h"

class DataDoc : public HaData
{
public:
    DECLARE_TM(DataDoc)

    DataDoc(year_t year);
    virtual ~DataDoc();

    void SetAccountIdAndNames(const std::vector<int64_t> &ids, const wxArrayString &names);

    year_t GetYear() const
    {
        return m_year;
    }

    const wxArrayString &GetAccountNames() const
    {
        return m_accountNames;
    }

    money_t GetRecordBalance(const record_t *record) const
    {
        if (record->udata != NULL) {
            return static_cast<ExtraCols *>(record->udata)->balance;
        }
        return 0;
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

    money_t GetStatIncome() const
    {
        return m_stat.income;
    }

    money_t GetStatOutlay() const
    {
        return m_stat.outlay;
    }

    int64_t GetRecordCount() const
    {
        int64_t count = 0;
        for (auto *pos = m_records.first; pos != nullptr; pos = pos->next) {
            if (get_record(pos)->flag == RECORD_FLAG_NORMAL) {
                ++count;
            }
        }
        return count;
    }

protected:
    static const wxString AccountGetter(const HaCsv *csv, const record_t *record, int i);
    static void AccountSetter(HaCsv *csv, record_t *record, int i, const wxString &value);

    bool AfterRead() override;

private:
    struct ExtraCols {
        money_t balance;
    };

    struct Stat {
        money_t opening;
        money_t closing;
        money_t income;
        money_t outlay;
    };

    static constexpr const int INVALID_ACCOUNT_ID = -1;
    static constexpr const char INVALID_ACCOUNT_NAME[] = "";

    struct Stat m_stat;

    year_t m_year;

    wxArrayString m_accountNames;
    BiMap<int64_t, wxString, INVALID_ACCOUNT_ID, INVALID_ACCOUNT_NAME> m_accountIdNameMap;
};

#endif /* _HA_DATA_DATA_DOC_H_ */
