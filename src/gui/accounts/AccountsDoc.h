#ifndef _HA_ACCOUNTS_ACCOUNTS_DOC_H_
#define _HA_ACCOUNTS_ACCOUNTS_DOC_H_

#include "../HaCsv.h"

#include "csv/str.h"

class AccountsDoc : public HaCsv
{
public:
    static constexpr int TYPE_COL = 0;
    static constexpr int ID_COL = 1;
    static constexpr int NAME_COL = 2;
    static constexpr int COLS = 3;

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
        return (const struct str *)get_const_field(&m_parser, record, NAME_COL);
    }

    void GetIdAndNames(std::vector<int64_t> &ids, wxArrayString &names) const;

protected:
    static const wxString TypeGetter(const HaCsv *csv, const record_t *record, int i);

    bool AfterRead() override;

    void SetNewRecord(record_t *record) override;
    bool IsRecordEmpty(record_t *record) override;

private:
    static constexpr const column_type COL_TYPES[COLS] = {
        CT_INT,
        CT_INT,
        CT_STR,
    };

    static constexpr const str COL_TITLES[COLS] = {
        {wxTRANSLATE("Type"), 4, false},
        {  wxTRANSLATE("Id"), 2, false},
        {wxTRANSLATE("Name"), 4, false},
    };

    mutable int m_maxId;

    static const wxArrayString &GetAccountTypeStrings();
};

#endif /* _HA_ACCOUNTS_ACCOUNTS_DOC_H_ */
