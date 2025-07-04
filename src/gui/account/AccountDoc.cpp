#include <wx/log.h>
#include <wx/translation.h>

#include "AccountDoc.h"

#include "csv/segment.h"

IMPLEMENT_TM(AccountDoc)

AccountDoc::AccountDoc() : CsvDoc(ACCOUNT_COLS, account_types, sizeof(struct account), account_get), m_maxId(0)
{
    wxLog::AddTraceMask(TM);
}

AccountDoc::~AccountDoc()
{
}

bool AccountDoc::AfterRead()
{
    const struct segment *wrong = fill_account_types(&m_segments, &m_maxId);
    if (wrong != NULL) {
        wxLogWarning(_("Invalid type: %s"), wrong->comment != NULL ? wrong->comment : "");
        return false;
    }
    return true;
}

void AccountDoc::SetNewItem(void *item)
{
    ((struct account *)item)->id = ++m_maxId;
}
