#include <wx/dc.h>

#include "DataGrid.h"

IMPLEMENT_DYNAMIC_CLASS(DataGrid, wxGrid);
IMPLEMENT_TM(DataGrid);

BEGIN_EVENT_TABLE(DataGrid, wxGrid)
EVT_GRID_SELECT_CELL(DataGrid::OnGridSelectCell)
END_EVENT_TABLE()

DataGrid::DataGrid() : wxGrid()
{
    wxLog::AddTraceMask(TM);
}

DataGrid::~DataGrid()
{
}

wxPen DataGrid::GetRowGridLinePen([[maybe_unused]] int row)
{
    return *wxTRANSPARENT_PEN;
}

wxPen DataGrid::GetColGridLinePen([[maybe_unused]] int col)
{
    return *wxLIGHT_GREY_PEN;
}

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

bool DataGrid::IsInsertEnabled() const
{
    return GetGridCursorRow() >= 0;
}

bool DataGrid::IsDeleteEnabled() const
{
    if (IsSelection()) {
        return true;
    }
    auto coords = GetGridCursorCoords();
    return coords != wxGridNoCellCoords && !GetCellAttr(coords)->IsReadOnly();
}

void DataGrid::SetAttributes()
{
    BeginBatch();
    SetColMinimalAcceptableWidth(80);
    SetRowMinimalAcceptableHeight(ROW_HEIGHT);
    auto logo = wxArtProvider::GetBitmap("logo");
    SetRowLabelSize(logo.GetWidth() + 2);
    SetColLabelSize(logo.GetHeight() + 2);
    DisableDragColMove();
    EndBatch();
}
