#include <wx/log.h>

#include "DataGrid.h"

IMPLEMENT_DYNAMIC_CLASS(DataGrid, HaGrid)

BEGIN_EVENT_TABLE(DataGrid, HaGrid)
END_EVENT_TABLE()

DataGrid::DataGrid() : HaGridTemplate<DataTable, DataDoc>()
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
    : HaGridTemplate<DataTable, DataDoc>(parent, id, pos, size, style, name)
{
    wxLog::AddTraceMask(TM);
}

DataGrid::~DataGrid()
{
}

void DataGrid::MakeDateVisible(const wxDateTime &date)
{
    auto *doc = GetTableDoc();
    int row = doc->FindDateRow(date.GetYear(), date.GetMonth() + 1, date.GetDay());
    if (row > 0) {
        MakeCellVisible(row, 0);
    }
}

int DataGrid::CursorColOfNewRow()
{
    return DataTable::OUTLAY_COL;
}
