#ifndef _HA_DATA_DATA_DOC_H_
#define _HA_DATA_DATA_DOC_H_

#include "../CsvDoc.h"

#include "csv/money.h"

class DataDoc : public CsvDoc
{
public:
    static const int COLS = 9;
    static const int DATE_COL = 0;
    static const int TIME_COL = 1;
    static const int ACCOUNT_COL = 2;
    static const int AMOUNT_COL = 3;
    static const int DESC_COL = 4;
    static const int REAL_AMOUNT_COL = 5;
    static const int REAL_DESC_COL = 6;
    static const int MEMO_COL = 7;
    static const int AUTO_SET_COL = 8;

    DECLARE_TM()

    DataDoc(int year);
    virtual ~DataDoc();

    money_t GetRecordRealAmount(const record_t *record) const
    {
        wxASSERT(record->flag == RECORD_FLAG_NORMAL);
        return *(money_t *)get_const_field(&m_parser, record, REAL_AMOUNT_COL);
    }

    money_t GetRecordBalance(const record_t *record) const
    {
        if (record->udata != NULL) {
            return static_cast<ExtraCols *>(record->udata)->balance;
        }
        return 0;
    }

    void SetRecordBalance(record_t *record, money_t balance)
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
        money_t m = GetRecordRealAmount(GetRecord(row));
        if (m < 0) {
            return GetMoneyString(-m);
        }
        return wxEmptyString;
    }

    const wxString GetOutlayString(int row) const
    {
        money_t m = GetRecordRealAmount(GetRecord(row));
        if (m > 0) {
            return GetMoneyString(m);
        }
        return wxEmptyString;
    }

    const wxString GetBalanceString(int row) const
    {
        money_t balance = GetRecordBalance(GetRecord(row));
        return GetMoneyString(balance);
    }

    void SetOpening(money_t opening);
    void UpdateBalanceStat();

    int FindDateRow(int year, int month, int day);

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

    static const column_type COL_TYPES[COLS];

    int m_year;
};

#endif /* _HA_DATA_DATA_DOC_H_ */
