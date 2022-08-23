#include <wx/msgdlg.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "../HaDocumentBase.h"
#include "Defs.h"
#include "RawApp.h"
#include "RawMainFrame.h"
#include "RawView.h"

IMPLEMENT_DYNAMIC_CLASS(RawMainFrame, wxDocParentFrame)
IMPLEMENT_TM(RawMainFrame)

BEGIN_EVENT_TABLE(RawMainFrame, wxDocParentFrame)
EVT_CLOSE(RawMainFrame::OnClose)
EVT_UPDATE_UI(ID_SECTION_ADD, RawMainFrame::OnUpdateSectionAdd)
EVT_UPDATE_UI(ID_SECTION_DELETE, RawMainFrame::OnUpdateSectionDelete)
EVT_UPDATE_UI(ID_CHANGE_PASS, RawMainFrame::OnUpdateChangePass)
EVT_MENU(ID_ABOUT, RawMainFrame::OnAbout)
END_EVENT_TABLE()

RawMainFrame::RawMainFrame(
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
}

void RawMainFrame::OnClose([[maybe_unused]] wxCloseEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    Destroy();
    wxExit();
}

void RawMainFrame::OnUpdateSectionAdd(wxUpdateUIEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    event.Enable(GetCurrentView() != nullptr);
}

void RawMainFrame::OnUpdateSectionDelete(wxUpdateUIEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    RawView *view = GetCurrentView();
    event.Enable(view != nullptr && view->IsSelected());
}

void RawMainFrame::OnUpdateChangePass(wxUpdateUIEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    RawView *view = GetCurrentView();
    event.Enable(view != nullptr);
}

void RawMainFrame::OnAbout([[maybe_unused]] wxCommandEvent &event)
{
    const wxString &appName = wxGetApp().GetAppDisplayName();
    wxMessageBox(appName + "\n(c) 2022, Lasy", wxString(_("About ")) + appName);
}

HaDocumentBase *RawMainFrame::GetCurrentDocument() const
{
    auto doc = GetDocumentManager()->GetCurrentDocument();
    return doc == nullptr ? nullptr : static_cast<HaDocumentBase *>(doc);
}

RawView *RawMainFrame::GetCurrentView() const
{
    auto view = GetDocumentManager()->GetCurrentView();
    return view == nullptr ? nullptr : static_cast<RawView *>(view);
}
