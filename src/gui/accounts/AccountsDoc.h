#ifndef _HA_ACCOUNTS_ACCOUNTS_DOC_H_
#define _HA_ACCOUNTS_ACCOUNTS_DOC_H_

#include "../HaCsv.h"

class AccountsDoc : public HaCsv
{
public:
    static constexpr int TYPE_COL = 0;
    static constexpr int ID_COL = 1;
    static constexpr int OWNER_COL = 2;
    static constexpr int BANK_COL = 3;
    static constexpr int NUMBER_COL = 4;
    static constexpr int OPEN_DATE_COL = 5;
    static constexpr int INITIAL_COL = 6;
    static constexpr int MEMO_COL = 7;
    static constexpr int COLS = 8;

    DECLARE_TM(AccountsDoc)

    AccountsDoc();
    virtual ~AccountsDoc();

    int64_t GetRecordId(const record_t *record) const
    {
        return *(int64_t *)get_const_field(&m_parser, record, ID_COL);
    }

    void SetRecordId(record_t *record, int64_t id) const
    {
        *(int64_t *)get_field(&m_parser, record, ID_COL) = id;
    }

    const struct str *GetRecordName(const record_t *record) const
    {
        return (const struct str *)get_const_field(&m_parser, record, OWNER_COL);
    }

    void GetIdAndNames(std::vector<int64_t> &ids, wxArrayString &names) const;

protected:
    static const wxString TypeGetter(const HaCsv *csv, const record_t *record, int i);
    static void TypeSetter(HaCsv *csv, record_t *record, int i, const wxString &value);

    bool AfterRead() override;

    void SetNewRecord(record_t *record) override;
    bool IsRecordEmpty(record_t *record) override;

private:
    static const column_type COL_TYPES[COLS];
    static const str COL_TITLES[COLS];

    mutable int m_maxId;

    static const wxArrayString &GetAccountTypeStrings();
};

#endif /* _HA_ACCOUNTS_ACCOUNTS_DOC_H_ */
