#include <wx/artprov.h>
#include <wx/dc.h>

#include "Defs.h"
#include "HaGrid.h"

IMPLEMENT_DYNAMIC_CLASS(HaGrid, wxGrid)
IMPLEMENT_TM(HaGrid)

BEGIN_EVENT_TABLE(HaGrid, wxGrid)
EVT_UPDATE_UI(ID_INSERT, HaGrid::OnUpdateInsert)
EVT_MENU(ID_INSERT, HaGrid::OnInsert)
EVT_UPDATE_UI(ID_DELETE, HaGrid::OnUpdateDelete)
EVT_MENU(ID_DELETE, HaGrid::OnDelete)
END_EVENT_TABLE()

HaGrid::HaGrid() : wxGrid()
{
    wxLog::AddTraceMask(TM);
}

HaGrid::HaGrid(
    wxWindow *parent,
    wxWindowID id,
    const wxPoint &pos,
    const wxSize &size,
    long style,
    const wxString &name
)
    : wxGrid(parent, id, pos, size, style, name)
{
    wxLog::AddTraceMask(TM);
}

HaGrid::~HaGrid()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    CheckEventHandler();
}

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

void HaGrid::OnUpdateInsert(wxUpdateUIEvent &event)
{
    // Cannot get focus if the grid is empty.
    event.Enable(true);
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

void HaGrid::OnUpdateDelete(wxUpdateUIEvent &event)
{
    event.Enable(HasFocus());
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

void HaGrid::CheckEventHandler()
{
    auto win = GetGridWindow();
    auto &children = win->GetChildren();
    for (auto &child : children) {
        auto *handler = child->GetEventHandler();
        if (handler != child) {
            wxLogTrace(
                TM,
                "Pushed event handler foud. win = \"%s\", class of handler = \"%s\"",
                child->GetName(),
                handler->GetClassInfo()->GetClassName()
            );
        }
    }
}
