#include <wx/log.h>

#include "DataGrid.h"

IMPLEMENT_TM(DataGrid)
IMPLEMENT_DYNAMIC_CLASS(DataGrid, HaGrid)

BEGIN_EVENT_TABLE(DataGrid, HaGrid)
END_EVENT_TABLE()

DataGrid::DataGrid() : HaGrid()
{
    wxLog::AddTraceMask(TM);
}

DataGrid::DataGrid(
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

DataGrid::~DataGrid()
{
}

DataDoc *DataGrid::GetTableDoc()
{
    return GetDataTable()->GetDataDoc();
}

HaTable *DataGrid::CreateHaTable(CsvDoc *doc)
{
    auto *ddoc = dynamic_cast<DataDoc *>(doc);
    wxASSERT(ddoc != nullptr);
    auto *table = new DataTable(ddoc);
    return table;
}
