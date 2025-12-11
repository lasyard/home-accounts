#include <wx/log.h>

#include "AccountsGrid.h"

#include "AccountsDoc.h"
#include "AccountsTable.h"

#include "../HaTable.h"

IMPLEMENT_TM(AccountsGrid)
IMPLEMENT_DYNAMIC_CLASS(AccountsGrid, HaGrid)

BEGIN_EVENT_TABLE(AccountsGrid, HaGrid)
END_EVENT_TABLE()

AccountsGrid::AccountsGrid() : HaGrid()
{
    wxLog::AddTraceMask(TM);
}

AccountsGrid::AccountsGrid(
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

AccountsGrid::~AccountsGrid()
{
}

HaTable *AccountsGrid::CreateHaTable(CsvDoc *doc)
{
    auto *d = dynamic_cast<AccountsDoc *>(doc);
    wxASSERT(d != nullptr);
    auto *table = new AccountsTable(d);
    return table;
}
