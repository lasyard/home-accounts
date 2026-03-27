#include <wx/log.h>

#include "DataGrid.h"

#include "DataTable.h"

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

void DataGrid::MakeDateVisible(const wxDateTime &date)
{
    auto *doc = static_cast<DataDoc *>(GetHaTable()->GetDoc());
    int row = doc->FindDateRow(date.GetYear(), date.GetMonth() + 1, date.GetDay());
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
