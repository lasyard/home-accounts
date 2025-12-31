#ifndef _HA_ACCOUNTS_ACCOUNTS_DOC_H_
#define _HA_ACCOUNTS_ACCOUNTS_DOC_H_

#include <wx/arrstr.h>

#include "../CsvDoc.h"

class AccountsDoc : public CsvDoc
{
public:
    static const int ACCOUNT_COLS = 6;
    static const int ACCOUNT_TYPE_COL = 0;
    static const int ACCOUNT_ID_COL = 1;
    static const int ACCOUNT_NAME_COL = 2;
    static const int ACCOUNT_BEGIN_COL = 3;
    static const int ACCOUNT_INITIAL_COL = 4;
    static const int ACCOUNT_MEMO_COL = 5;

    DECLARE_TM()

    AccountsDoc();
    virtual ~AccountsDoc();

    const wxString GetValueString(int pos, int i) const override;
    void SetValueString(int pos, int i, const wxString &value) override;

private:
    static const column_type ACCOUNT_COL_TYPES[ACCOUNT_COLS];

    static const wxArrayString &GetAccountTypeStrings();

    mutable int m_maxId;

    void SetNewRecord(record_t *record) override;
};

#endif /* _HA_ACCOUNTS_ACCOUNTS_DOC_H_ */
