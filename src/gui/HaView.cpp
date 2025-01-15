#include <wx/notebook.h>

#include "HaView.h"

#include "HaApp.h"
#include "HaDefs.h"
#include "HaDocument.h"
#include "HaMainFrame.h"
#include "Utils.h"

#include "panel/HaRawPanel.h"

IMPLEMENT_DYNAMIC_CLASS(HaView, wxView)
IMPLEMENT_TM(HaView)

BEGIN_EVENT_TABLE(HaView, wxView)
EVT_UPDATE_UI(ID_INSERT, HaView::OnUpdateMenu)
EVT_MENU(ID_INSERT, HaView::OnMenu)
EVT_UPDATE_UI(wxID_DELETE, HaView::OnUpdateMenu)
EVT_MENU(wxID_DELETE, HaView::OnMenu)
END_EVENT_TABLE()

HaView::HaView() : wxView()
{
    wxLog::AddTraceMask(TM);
}

HaView::~HaView()
{
}

bool HaView::OnCreate([[maybe_unused]] wxDocument *doc, [[maybe_unused]] long flags)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto *frame = static_cast<HaMainFrame *>(wxGetApp().GetTopWindow());
    SetFrame(frame);
    m_book = frame->GetBook();
    m_book->Show();
    frame->Layout();
    m_book->AddPage(new HaRawPanel(m_book), _("raw"));
    Activate(true);
    return true;
}

bool HaView::OnClose(bool deleteWindow)
{
    Activate(false);
    auto *frame = static_cast<HaMainFrame *>(GetFrame());
    frame->SetTitle("");
    m_book->DeleteAllPages();
    m_book->Show(false);
    return wxView::OnClose(deleteWindow);
}

void HaView::OnUpdate([[maybe_unused]] wxView *sender, [[maybe_unused]] wxObject *hint)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    GetCurrentPanel()->SetDocument(GetHaDocument());
    GetCurrentPanel()->OnUpdate();
}

void HaView::OnDraw([[maybe_unused]] wxDC *dc)
{
    // Do nothing.
}

void HaView::OnClosingDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
}

void HaView::OnUpdateMenu(wxUpdateUIEvent &event)
{
    Utils::DelegateEvent(GetCurrentPanel(), event);
}

void HaView::OnMenu(wxCommandEvent &event)
{
    Utils::DelegateEvent(GetCurrentPanel(), event);
}

void HaView::SaveContents()
{
    GetCurrentPanel()->SaveContents();
}

HaDocument *HaView::GetHaDocument() const
{
    return dynamic_cast<HaDocument *>(GetDocument());
}

HaPanel *HaView::GetCurrentPanel() const
{
    return dynamic_cast<HaPanel *>(m_book->GetCurrentPage());
}
