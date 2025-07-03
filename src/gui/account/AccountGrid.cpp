#include <wx/log.h>

#include "AccountGrid.h"

#include "AccountDoc.h"
#include "AccountTable.h"

#include "../HaTable.h"

IMPLEMENT_TM(AccountGrid)
IMPLEMENT_DYNAMIC_CLASS(AccountGrid, HaGrid)

BEGIN_EVENT_TABLE(AccountGrid, HaGrid)
END_EVENT_TABLE()

AccountGrid::AccountGrid() : HaGrid()
{
    wxLog::AddTraceMask(TM);
}

AccountGrid::AccountGrid(
    wxWindow *parent,
    wxWindowID id,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name
)
    : HaGrid(parent, id, pos, size, style, name)
{
    wxLog::AddTraceMask(TM);
}

AccountGrid::~AccountGrid()
{
}

HaTable *AccountGrid::CreateHaTable(CsvDoc *doc)
{
    auto *adoc = dynamic_cast<AccountDoc *>(doc);
    wxASSERT(adoc != nullptr);
    auto *table = new AccountTable(adoc);
    return table;
}
