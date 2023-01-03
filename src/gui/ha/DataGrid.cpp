#include <wx/artprov.h>
#include <wx/dc.h>

#include "DataGrid.h"

IMPLEMENT_DYNAMIC_CLASS(DataGrid, wxGrid);

BEGIN_EVENT_TABLE(DataGrid, wxGrid)
EVT_GRID_SELECT_CELL(DataGrid::onGridSelectCell)
END_EVENT_TABLE()

void DataGrid::DrawCornerLabel(wxDC &dc)
{
    dc.DrawBitmap(wxArtProvider::GetBitmap("logo"), 1, 1);
}

void DataGrid::onGridSelectCell(wxGridEvent &event)
{
    int row = event.GetRow();
    int numRows, numCols;
    // GetCellSize never returns CellSpan_Inside, so test the first column.
    if (GetCellSize(row, 0, &numRows, &numCols) == CellSpan_Main) {
        SelectRow(row);
        event.Veto();
    }
}

void DataGrid::SetAttributes()
{
    BeginBatch();
    SetColMinimalAcceptableWidth(80);
    SetRowMinimalAcceptableHeight(18);
    SetRowLabelSize(0);
    SetColLabelSize(wxGRID_AUTOSIZE);
    AutoSizeColumns(false);
    AutoSizeRows(false);
    DisableDragColMove();
    EndBatch();
}
