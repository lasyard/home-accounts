#include <wx/log.h>

#include "DataGrid.h"

#include "DataTable.h"

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

DataTable *DataGrid::GetDataTable() const
{
    return static_cast<DataTable *>(GetTable());
}

DataDoc *DataGrid::GetTableDoc()
{
    return GetDataTable()->GetDataDoc();
}

void DataGrid::MakeDateVisible(const wxDateTime &date)
{
    int row = GetTableDoc()->FindDateRow(date.GetYear(), date.GetMonth() + 1, date.GetDay());
    if (row > 0) {
        MakeCellVisible(row, 0);
    }
}

HaTable *DataGrid::CreateHaTable(CsvDoc *doc)
{
    return Utils::CreateHaTable<DataTable, DataDoc>(doc);
}

int DataGrid::CursorColOfNewRow()
{
    return DataTable::OUTLAY_COL;
}
