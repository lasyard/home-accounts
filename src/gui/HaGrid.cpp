#include <fstream>

#include <wx/artprov.h>
#include <wx/dc.h>
#include <wx/filedlg.h>
#include <wx/log.h>
#include <wx/msgdlg.h>

#include "HaGrid.h"

#include "CachedTable.h"
#include "Defs.h"

#include "data/Dao.h"

IMPLEMENT_DYNAMIC_CLASS(HaGrid, wxGrid)
IMPLEMENT_TM(HaGrid)

BEGIN_EVENT_TABLE(HaGrid, wxGrid)
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

bool HaGrid::ImportFile(const wxString &what)
{
    auto answer = wxMessageBox(_("Overwrite the existing ") + what + _("?"), _("Confirm"), wxYES_NO | wxCENTER);
    if (answer == wxYES) {
        auto fileName = wxLoadFileSelector(_("CSV file"), "CSV file (*.csv)|*.csv|Text file(*.txt)|*.txt");
        if (!fileName.IsEmpty()) {
            std::ifstream is(fileName.ToStdString());
            auto table = GetCachedTable();
            if (table != nullptr) {
                table->GetDao()->read(is);
                // newTable and table share the same dao.
                auto newTable = table->Clone();
                SetTable(newTable, true, GetSelectionMode());
                RefreshContent();
                return true;
            }
        }
    }
    return false;
}

void HaGrid::ExportTable(const wxString &what)
{
    auto table = GetCachedTable();
    wxString nameHint = table->GetDaoName();
    nameHint.Replace('/', '_');
    auto realName = wxSaveFileSelector(what, "CSV File (*.csv)|*.csv", nameHint + ".csv");
    if (!realName.IsEmpty()) {
        std::ofstream os(realName.ToStdString());
        table->GetDao()->write(os);
    }
}

CachedTable *HaGrid::GetCachedTable()
{
    return dynamic_cast<CachedTable *>(GetTable());
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
