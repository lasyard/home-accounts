#include <wx/log.h>
#include <wx/translation.h>

#include "AccountsDoc.h"

IMPLEMENT_TM(AccountsDoc)

const column_type AccountsDoc::ACCOUNT_COL_TYPES[] = {
    CT_INT,
    CT_STR,
    CT_DATE,
    CT_MONEY,
    CT_STR,
};

const wxArrayString AccountsDoc::ACCOUNT_TYPE_STRINGS = {
    _("Invalid"),
    _("Debit"),
    _("Credit"),
    _("Transfer"),
};

AccountsDoc::AccountsDoc() : CsvDoc(ACCOUNT_COLS, ACCOUNT_COL_TYPES), m_maxId(0)
{
    wxLog::AddTraceMask(TM);
}

AccountsDoc::~AccountsDoc()
{
}

void AccountsDoc::SetNewRecord(record_t *record)
{
    *(int64_t *)get_field(&m_parser, record, ACCOUNT_ID_COL) = ++m_maxId;
}
