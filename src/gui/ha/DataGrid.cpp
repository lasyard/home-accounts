#include <wx/artprov.h>
#include <wx/dc.h>

#include "DataGrid.h"
#include "DataTable.h"
#include "Defs.h"

IMPLEMENT_DYNAMIC_CLASS(DataGrid, wxGrid);
IMPLEMENT_TM(DataGrid);

BEGIN_EVENT_TABLE(DataGrid, wxGrid)
EVT_GRID_SELECT_CELL(DataGrid::OnGridSelectCell)
END_EVENT_TABLE()

void DataGrid::DrawCornerLabel(wxDC &dc)
{
    dc.DrawBitmap(wxArtProvider::GetBitmap("logo"), 1, 1);
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

void DataGrid::SetAttributes()
{
    BeginBatch();
    SetColMinimalAcceptableWidth(80);
    SetRowMinimalAcceptableHeight(ROW_HEIGHT);
    SetRowLabelSize(wxArtProvider::GetBitmap("logo").GetWidth() + 2);
    SetColLabelSize(wxArtProvider::GetBitmap("logo").GetHeight() + 2);
    DisableDragColMove();
    EndBatch();
}

void DataGrid::AutoFit()
{
    BeginBatch();
    AutoSizeRows(false);
    AutoSizeColumns(false);
    EndBatch();
}
