#include <wx/log.h>
#include <wx/translation.h>

#include "AccountDoc.h"

#include "csv/segment.h"

#include "data/account.h"

IMPLEMENT_TM(AccountDoc)

AccountDoc::AccountDoc() : CsvDoc(ACCOUNT_COLS, account_types, sizeof(struct account), account_get)
{
    wxLog::AddTraceMask(TM);
}

AccountDoc::~AccountDoc()
{
}

bool AccountDoc::AfterRead()
{
    const struct segment *wrong = fill_account_types(&m_segments);
    if (wrong != NULL) {
        wxLogWarning(_("Invalid type: %s"), wrong->comment != NULL ? wrong->comment : "");
        return false;
    }
    return true;
}
