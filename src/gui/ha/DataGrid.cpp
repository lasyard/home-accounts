#include <wx/artprov.h>
#include <wx/dc.h>

#include "DataGrid.h"

IMPLEMENT_DYNAMIC_CLASS(DataGrid, wxGrid);

BEGIN_EVENT_TABLE(DataGrid, wxGrid)
END_EVENT_TABLE()

void DataGrid::DrawCornerLabel(wxDC &dc)
{
    dc.DrawBitmap(wxArtProvider::GetBitmap("logo"), 1, 1);
}

void DataGrid::SetAttributes()
{
    BeginBatch();
    SetColMinimalAcceptableWidth(80);
    SetRowMinimalAcceptableHeight(18);
    wxBitmap logo = wxArtProvider::GetBitmap("logo");
    if (GetRowLabelSize() < logo.GetWidth() + 2) {
        SetRowLabelSize(logo.GetWidth() + 2);
    }
    if (GetColLabelSize() < logo.GetHeight() + 2) {
        SetColLabelSize(logo.GetHeight() + 2);
    }
    SetRowLabelSize(wxGRID_AUTOSIZE);
    SetColLabelSize(wxGRID_AUTOSIZE);
    AutoSizeColumns(false);
    AutoSizeRows(false);
    DisableDragColMove();
    EndBatch();
}
