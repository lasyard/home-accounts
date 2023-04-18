#include "DataGrid.h"
#include "../Defs.h"

IMPLEMENT_DYNAMIC_CLASS(DataGrid, HaGrid);
IMPLEMENT_TM(DataGrid);

BEGIN_EVENT_TABLE(DataGrid, wxGrid)
EVT_GRID_SELECT_CELL(DataGrid::OnGridSelectCell)
EVT_UPDATE_UI(ID_INSERT, DataGrid::OnUpdateInsert)
EVT_MENU(ID_INSERT, DataGrid::OnInsert)
EVT_UPDATE_UI(wxID_DELETE, DataGrid::OnUpdateDelete)
EVT_MENU(wxID_DELETE, DataGrid::OnDelete)
END_EVENT_TABLE()

DataGrid::DataGrid() : HaGrid()
{
    wxLog::AddTraceMask(TM);
}

DataGrid::~DataGrid()
{
}

void DataGrid::OnGridSelectCell(wxGridEvent &event)
{
    int row = event.GetRow();
    int col = event.GetCol();
    int numRows, numCols;
    // GetCellSize never returns CellSpan_Inside, so test the first column.
    if (col != 0 && GetCellSize(row, 0, &numRows, &numCols) == CellSpan_Main) {
        // This will re-generate this event.
        SetGridCursor(row, 0);
        event.Veto();
    }
}

void DataGrid::OnUpdateInsert(wxUpdateUIEvent &event)
{
    event.Enable(HasFocus() && GetGridCursorRow() >= 0);
}

void DataGrid::OnInsert([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    BeginBatch();
    auto row = GetGridCursorRow();
    InsertRows(row + 1);
    // `AutoSizeRow` not work here.
    AutoSizeRows(false);
    SetGridCursor(row + 1, DataTable::OUTLAY_COL);
    EndBatch();
}

void DataGrid::OnUpdateDelete(wxUpdateUIEvent &event)
{
    if (HasFocus()) {
        if (IsSelection()) {
            event.Enable(true);
        } else {
            auto coords = GetGridCursorCoords();
            // Vital, use `GetCellAttrPtr` instead of `GetCellAttr`.
            event.Enable(coords != wxGridNoCellCoords && !GetCellAttrPtr(coords)->IsReadOnly());
        }
    } else {
        event.Enable(false);
    }
}

void DataGrid::OnDelete([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    BeginBatch();
    const auto blocks = GetSelectedBlocks();
    if (blocks.begin() != blocks.end()) { // Not empty
        for (const auto block : GetSelectedBlocks()) {
            auto topLeft = block.GetTopLeft();
            auto bottomRight = block.GetBottomRight();
            for (int i = topLeft.GetRow(); i <= bottomRight.GetRow(); ++i) {
                for (int j = topLeft.GetCol(); j <= bottomRight.GetCol(); ++j) {
                    SetCellValue(i, j, "");
                }
            }
        }
        ClearSelection();
    } else {
        SetCellValue(GetGridCursorCoords(), "");
    }
    EndBatch();
}
