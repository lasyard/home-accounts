#include <wx/log.h>

#include "AccountsGrid.h"

IMPLEMENT_DYNAMIC_CLASS(AccountsGrid, HaGrid)

BEGIN_EVENT_TABLE(AccountsGrid, HaGrid)
END_EVENT_TABLE()

AccountsGrid::AccountsGrid() : HaGridTemplate<AccountsTable, AccountsDoc>()
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
    : HaGridTemplate<AccountsTable, AccountsDoc>(parent, id, pos, size, style, name)
{
    wxLog::AddTraceMask(TM);
}

AccountsGrid::~AccountsGrid()
{
}

int AccountsGrid::CursorColOfNewRow()
{
    return AccountsTable::NAME_COL;
}
