#include <wx/log.h>
#include <wx/translation.h>

#include "AccountsDoc.h"

IMPLEMENT_TM(AccountsDoc)

const column_type AccountsDoc::ACCOUNT_COL_TYPES[] = {
    CT_INT,
    CT_INT,
    CT_STR,
    CT_DATE,
    CT_MONEY,
    CT_STR,
};

const wxArrayString &AccountsDoc::GetAccountTypeStrings()
{
    static wxArrayString s;
    if (s.IsEmpty()) {
        s.Add(_("Invalid"));
        s.Add(_("Debit"));
        s.Add(_("Credit"));
        s.Add(_("Transfer"));
    }
    return s;
}

AccountsDoc::AccountsDoc() : CsvDoc(ACCOUNT_COLS, ACCOUNT_COL_TYPES, 1), m_maxId(0)
{
    wxLog::AddTraceMask(TM);
}

AccountsDoc::~AccountsDoc()
{
}

const wxString AccountsDoc::GetValueString(int pos, int i) const
{
    if (i == ACCOUNT_TYPE_COL) {
        int64_t type = *(int64_t *)get_const_field(&m_parser, GetRecord(pos), i);
        const wxArrayString &types = GetAccountTypeStrings();
        if (type < 0 || type >= (int64_t)types.size()) {
            type = 0;
        }
        return types[type];
    }
    return CsvDoc::GetValueString(pos, i);
}

void AccountsDoc::SetValueString(int pos, int i, const wxString &value)
{
    if (i == ACCOUNT_TYPE_COL) {
        int64_t index;
        const wxArrayString &types = GetAccountTypeStrings();
        for (index = 0; index < (int64_t)types.size(); ++index) {
            if (types[index] == value) {
                break;
            }
        }
        if (index == (int64_t)types.size()) {
            index = 0;
        }
        *(int64_t *)get_field(&m_parser, GetRecord(pos), i) = index;
    } else {
        CsvDoc::GetValueString(pos, i);
    }
}

void AccountsDoc::SetNewRecord(record_t *record)
{
    *(int64_t *)get_field(&m_parser, record, ACCOUNT_ID_COL) = ++m_maxId;
}
