#include <wx/artprov.h>
#include <wx/dc.h>

#include "HaGrid.h"

wxPen HaGrid::GetRowGridLinePen([[maybe_unused]] int row)
{
    return *wxTRANSPARENT_PEN;
}

wxPen HaGrid::GetColGridLinePen([[maybe_unused]] int col)
{
    return *wxTRANSPARENT_PEN;
}

void HaGrid::DrawCornerLabel(wxDC &dc)
{
    dc.DrawBitmap(wxArtProvider::GetBitmap("logo"), 1, 1);
}

void HaGrid::SetAttributes()
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

void HaGrid::OnInsert([[maybe_unused]] wxCommandEvent &event)
{
    BeginBatch();
    int row = GetGridCursorRow();
    if (row < 0 || row == GetNumberRows() - 1) {
        AppendRows();
    } else {
        InsertRows(row + 1);
    }
    SetGridCursor(row + 1, GetGridCursorCol());
    AutoFit();
    EndBatch();
}

int reverseInt(int a, int b)
{
    if (a > b) {
        return -1;
    } else if (a < b) {
        return 1;
    }
    return 0;
}

void HaGrid::OnDelete([[maybe_unused]] wxCommandEvent &event)
{
    BeginBatch();
    if (GetSelectionMode() == wxGrid::wxGridSelectionModes::wxGridSelectRows) {
        const auto &blocks = GetSelectedRowBlocks();
        for (auto i = blocks.rbegin(); i != blocks.rend(); ++i) {
            for (auto j = i->GetBottomRow(); j >= i->GetTopRow(); --j) {
                DeleteRows(j);
            }
        }
    }
    AutoFit();
    EndBatch();
}
