#ifndef _HA_ACCOUNTS_ACCOUNTS_DOC_H_
#define _HA_ACCOUNTS_ACCOUNTS_DOC_H_

#include <wx/arrstr.h>

#include "../CsvDoc.h"

class AccountsDoc : public CsvDoc
{
public:
    static const int COLS = 7;
    static const int TYPE_COL = 0;
    static const int ID_COL = 1;
    static const int NAME_COL = 2;
    static const int BANK_COL = 3;
    static const int OPEN_DATE_COL = 4;
    static const int INITIAL_COL = 5;
    static const int MEMO_COL = 6;

    DECLARE_TM()

    AccountsDoc();
    virtual ~AccountsDoc();

    int GetRecordId(const record_t *record) const
    {
        return *(int64_t *)get_const_field(&m_parser, record, ID_COL);
    }

    void SetRecordId(record_t *record, int id) const
    {
        *(int64_t *)get_field(&m_parser, record, ID_COL) = id;
    }

    const struct str *GetRecordName(const record_t *record) const
    {
        return (struct str *)get_const_field(&m_parser, record, NAME_COL);
    }

protected:
    static const wxString TypeGetter(const struct parser *parser, const record_t *record, int i);
    static void TypeSetter(const struct parser *parser, record_t *record, int i, const wxString &value);

    bool AfterRead() override;

    void SetNewRecord(record_t *record) override;
    bool IsRecordEmpty(record_t *record) override;

private:
    static const column_type COL_TYPES[COLS];

    mutable int m_maxId;

    static const wxArrayString &GetAccountTypeStrings();
};

#endif /* _HA_ACCOUNTS_ACCOUNTS_DOC_H_ */
