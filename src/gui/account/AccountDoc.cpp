#include <wx/log.h>
#include <wx/translation.h>

#include "AccountDoc.h"

#include "csv/segment.h"

IMPLEMENT_TM(AccountDoc)

const wxArrayString AccountDoc::ACCOUNT_TYPE_STRINGS = {
    TT("Invalid"),
    TT("Debit"),
    TT("Credit"),
    TT("Transfer"),
};

AccountDoc::AccountDoc() : CsvDoc(ACCOUNT_COLS, account_types, sizeof(struct account), account_get), m_maxId(0)
{
    wxLog::AddTraceMask(TM);
}

AccountDoc::~AccountDoc()
{
}

const wxString AccountDoc::GetSegmentValueString(const struct segment *segment) const
{
    int n = get_segment_int(segment);
    return 1 <= n && n < (int)ACCOUNT_TYPE_STRINGS.size() ? _(ACCOUNT_TYPE_STRINGS[n]) : _("Invalid");
}

void AccountDoc::SetSegmentValueString([[maybe_unused]] struct segment *segment, [[maybe_unused]] const wxString &value)
{
    wxLogError(_("This value is readonly"));
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
