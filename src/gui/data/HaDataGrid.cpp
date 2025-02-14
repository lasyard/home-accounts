#include <wx/log.h>

#include "HaDataGrid.h"

#include "HaDataTable.h"

IMPLEMENT_TM(HaDataGrid)
IMPLEMENT_DYNAMIC_CLASS(HaDataGrid, HaGrid)

BEGIN_EVENT_TABLE(HaDataGrid, HaGrid)
END_EVENT_TABLE()

HaDataGrid::HaDataGrid() : HaGrid()
{
    wxLog::AddTraceMask(TM);
}

HaDataGrid::HaDataGrid(
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

HaDataGrid::~HaDataGrid()
{
}

wxGridTableBase *HaDataGrid::CreateHaTable(CsvDoc *doc)
{
    return new HaDataTable(doc);
}
