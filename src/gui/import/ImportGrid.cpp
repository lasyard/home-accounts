#include <wx/log.h>

#include "ImportGrid.h"

IMPLEMENT_DYNAMIC_CLASS(ImportGrid, HaGrid)

BEGIN_EVENT_TABLE(ImportGrid, HaGrid)
END_EVENT_TABLE()

ImportGrid::ImportGrid() : HaGridTemplate<ImportTable, ImportDoc>()
{
    wxLog::AddTraceMask(TM);
}

ImportGrid::ImportGrid(
    wxWindow *parent,
    wxWindowID id,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name
)
    : HaGridTemplate<ImportTable, ImportDoc>(parent, id, pos, size, style, name)
{
    wxLog::AddTraceMask(TM);
}

ImportGrid::~ImportGrid()
{
}
