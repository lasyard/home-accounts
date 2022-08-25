#include <wx/msgdlg.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "DataGrid.h"
#include "Defs.h"
#include "HaApp.h"
#include "HaDocument.h"
#include "HaMainFrame.h"
#include "HaView.h"

IMPLEMENT_DYNAMIC_CLASS(HaMainFrame, wxDocParentFrame)
IMPLEMENT_TM(HaMainFrame)

BEGIN_EVENT_TABLE(HaMainFrame, wxDocParentFrame)
EVT_CLOSE(HaMainFrame::OnClose)
EVT_UPDATE_UI(ID_CHANGE_PASS, HaMainFrame::OnUpdateChangePass)
EVT_MENU(ID_ABOUT, HaMainFrame::OnAbout)
END_EVENT_TABLE()

HaMainFrame::HaMainFrame(
    wxDocManager *manager,
    wxFrame *parent,
    wxWindowID id,
    const wxString &title,
    const wxPoint &pos,
    const wxSize &size,
    long type,
    const wxString &name
)
    : wxDocParentFrame(manager, parent, id, title, pos, size, type, name)
{
    wxXmlResource::Get()->LoadObject(this, nullptr, "main", "wxFrame");
    m_transactionsGrid = XRCCTRL(*this, "transactions", DataGrid);
    m_transactionsGrid->SetAttributes();
}

void HaMainFrame::OnClose([[maybe_unused]] wxCloseEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    Destroy();
    wxExit();
}

void HaMainFrame::OnUpdateChangePass(wxUpdateUIEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    HaView *view = GetCurrentView();
    event.Enable(view != nullptr);
}

void HaMainFrame::OnAbout([[maybe_unused]] wxCommandEvent &event)
{
    const wxString &appName = wxGetApp().GetAppDisplayName();
    wxMessageBox(appName + "\n(c) 2022, Lasy", wxString(_("About ")) + appName);
}

HaDocument *HaMainFrame::GetCurrentDocument() const
{
    auto doc = GetDocumentManager()->GetCurrentDocument();
    return doc == nullptr ? nullptr : static_cast<HaDocument *>(doc);
}

HaView *HaMainFrame::GetCurrentView() const
{
    auto view = GetDocumentManager()->GetCurrentView();
    return view == nullptr ? nullptr : static_cast<HaView *>(view);
}
