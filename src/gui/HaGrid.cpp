#include <wx/artprov.h>

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
