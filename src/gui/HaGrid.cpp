#include <wx/artprov.h>
#include <wx/dc.h>
#include <wx/filedlg.h>
#include <wx/log.h>
#include <wx/msgdlg.h>

#include "HaGrid.h"

#include "Algos.h"
#include "HaDefs.h"
#include "HaTable.h"

IMPLEMENT_TM(HaGrid)

BEGIN_EVENT_TABLE(HaGrid, wxGrid)
EVT_UPDATE_UI(ID_INSERT, HaGrid::OnUpdateInsert)
EVT_MENU(ID_INSERT, HaGrid::OnInsert)
EVT_UPDATE_UI(wxID_DELETE, HaGrid::OnUpdateDelete)
EVT_MENU(wxID_DELETE, HaGrid::OnDelete)
EVT_GRID_SELECT_CELL(HaGrid::OnSelectCell)
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

// `wxTRANSPARENT_PEN` is problematic on Windows
wxPen HaGrid::GetRowGridLinePen([[maybe_unused]] int row)
{
    return *wxLIGHT_GREY_PEN;
}

wxPen HaGrid::GetColGridLinePen([[maybe_unused]] int col)
{
    return *wxLIGHT_GREY_PEN;
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

void HaGrid::InitTable(CsvDoc *doc)
{
    auto *table = CreateHaTable(doc);
    table->Init();
    SetTable(table, true);
    RefreshContent();
}

void HaGrid::SaveTable(std::ostream &os)
{
    SaveEditControlValue();
    auto *table = GetHaTable();
    if (table != nullptr) {
        table->SaveTo(os);
    }
}

CsvDoc *HaGrid::GetTableDoc()
{
    auto *table = GetHaTable();
    return table != nullptr ? table->GetCsvDoc() : nullptr;
}

void HaGrid::OnUpdateInsert(wxUpdateUIEvent &event)
{
    event.Enable(HasFocus() && GetGridCursorRow() >= 0);
}

void HaGrid::OnInsert([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    BeginBatch();
    auto row = GetGridCursorRow();
    if (InsertRows(row + 1)) {
        AutoSizeRow(row + 1);
        SetGridCursor(row + 1, 1);
    }
    EndBatch();
}

void HaGrid::OnUpdateDelete(wxUpdateUIEvent &event)
{
    if (HasFocus()) {
        if (IsSelection()) {
            event.Enable(true);
        } else {
            auto coords = GetGridCursorCoords();
            event.Enable(coords != wxGridNoCellCoords);
        }
    } else {
        event.Enable(false);
    }
}

void HaGrid::OnDelete([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    BeginBatch();
    if (IsSelection()) {
        auto blocks = GetSelectedBlocks();
        if (SelectionIsWholeRow(blocks)) {
            DeleteAllRowsInBlocks(blocks);
        } else {
            ClearAllCellsInBlocks(blocks);
        }
    } else {
        SafeClearCell(GetGridCursorCoords());
    }
    EndBatch();
}

void HaGrid::OnSelectCell(wxGridEvent &event)
{
    int row = event.GetRow();
    int col = event.GetCol();
    auto *table = static_cast<const HaTable *>(GetTable());
    if (table->GetRowType(row) == HaTableIndex::SEGMENT) {
        if (col > 0) {
            event.Veto();
            SetGridCursor(row, 0);
        }
    }
    event.Skip();
}

bool HaGrid::SelectionIsWholeRow(const wxGridBlocks &blocks)
{
    for (auto &block : blocks) {
        for (int i = block.GetTopRow(); i <= block.GetBottomRow(); ++i) {
            if (block.GetLeftCol() != 0 || block.GetRightCol() != GetNumberCols() - 1) {
                return false;
            }
        }
    }
    return true;
}

void HaGrid::DeleteAllRowsInBlocks(const wxGridBlocks &blocks)
{
    std::vector<std::pair<int, int>> rows;
    for (auto &block : blocks) {
        rows.push_back(std::make_pair(block.GetTopRow(), block.GetBottomRow()));
    }
    std::vector<std::pair<int, int>> sorted;
    Algos::MergeRange(rows, sorted);
    for (auto i = sorted.crbegin(); i != sorted.crend(); ++i) {
        DeleteRows(i->first, i->second - i->first + 1);
    }
}

void HaGrid::ClearAllCellsInBlocks(const wxGridBlocks &blocks)
{
    for (auto &block : blocks) {
        for (int i = block.GetTopRow(); i <= block.GetBottomRow(); ++i) {
            for (int j = block.GetLeftCol(); j <= block.GetRightCol(); ++j) {
                SafeClearCell(wxGridCellCoords(i, j));
            }
        }
    }
}

HaTable *HaGrid::GetHaTable()
{
    return dynamic_cast<HaTable *>(GetTable());
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
