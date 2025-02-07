#include <wx/log.h>

#include "HaDataGrid.h"

#include "HaDataTable.h"

IMPLEMENT_TM(HaDataGrid)
IMPLEMENT_DYNAMIC_CLASS(HaDataGrid, HaGrid)

BEGIN_EVENT_TABLE(HaDataGrid, HaGrid)
EVT_GRID_SELECT_CELL(HaDataGrid::OnSelectCell)
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

void HaDataGrid::OnSelectCell(wxGridEvent &event)
{
    int row = event.GetRow();
    int col = event.GetCol();
    auto *table = static_cast<const HaDataTable *>(GetTable());
    if (table->GetRowType(row) == HaDataTable::SEGMENT) {
        if (col > 0) {
            event.Veto();
            SetGridCursor(row, 0);
        }
    }
    event.Skip();
}

wxGridTableBase *HaDataGrid::CreateHaTable(CsvDoc *doc)
{
    return new HaDataTable(doc);
}
