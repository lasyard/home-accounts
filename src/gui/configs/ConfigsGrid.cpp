#include <wx/log.h>

#include "ConfigsGrid.h"

#include "../Defs.h"

IMPLEMENT_DYNAMIC_CLASS(ConfigsGrid, HaGrid)
IMPLEMENT_TM(ConfigsGrid)

BEGIN_EVENT_TABLE(ConfigsGrid, HaGrid)
EVT_UPDATE_UI(ID_INSERT, ConfigsGrid::OnUpdateInsert)
EVT_MENU(ID_INSERT, ConfigsGrid::OnInsert)
EVT_UPDATE_UI(wxID_DELETE, ConfigsGrid::OnUpdateDelete)
EVT_MENU(wxID_DELETE, ConfigsGrid::OnDelete)
END_EVENT_TABLE()

ConfigsGrid::ConfigsGrid() : HaGrid()
{
    wxLog::AddTraceMask(TM);
}

ConfigsGrid::ConfigsGrid(
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

ConfigsGrid::~ConfigsGrid()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
}

void ConfigsGrid::OnUpdateInsert(wxUpdateUIEvent &event)
{
    // Cannot get focus if the grid is empty.
    event.Enable(true);
}

void ConfigsGrid::OnInsert([[maybe_unused]] wxCommandEvent &event)
{
    BeginBatch();
    int row = GetGridCursorRow();
    bool success = false;
    if (row < 0 || row == GetNumberRows() - 1) {
        success = AppendRows();
    } else {
        success = InsertRows(row + 1);
    }
    if (success) {
        SetGridCursor(row + 1, GetGridCursorCol());
    }
    EndBatch();
}

void ConfigsGrid::OnUpdateDelete(wxUpdateUIEvent &event)
{
    event.Enable(HasFocus());
}

void ConfigsGrid::OnDelete([[maybe_unused]] wxCommandEvent &event)
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
    EndBatch();
}
