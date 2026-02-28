#include <wx/notebook.h>

#include "HaView.h"

#include "HaApp.h"
#include "HaDefs.h"
#include "HaDocument.h"
#include "HaMainFrame.h"
#include "Utils.h"

#include "accounts/AccountsPanel.h"
#include "data/DataPanel.h"
#include "import/ImportPanel.h"
#include "raw/RawPanel.h"

IMPLEMENT_DYNAMIC_CLASS(HaView, wxView)
IMPLEMENT_TM(HaView)

BEGIN_EVENT_TABLE(HaView, wxView)
EVT_UPDATE_UI(ID_INSERT, HaView::OnUpdateMenu)
EVT_MENU(ID_INSERT, HaView::OnMenu)
EVT_UPDATE_UI(wxID_DELETE, HaView::OnUpdateMenu)
EVT_MENU(wxID_DELETE, HaView::OnMenu)
EVT_MENU(ID_IMPORT, HaView::OnImport)
EVT_NOTEBOOK_PAGE_CHANGING(ID_BOOK, HaView::OnPageChanging)
EVT_NOTEBOOK_PAGE_CHANGED(ID_BOOK, HaView::OnPageChanged)
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
    m_book->AddPage(new DataPanel(m_book), _("Data"));
    m_book->AddPage(new AccountsPanel(m_book), _("Accounts"));
    m_book->AddPage(new RawPanel(m_book), _("Raw"));
    Activate(true);
    m_importPanel = nullptr;
    return true;
}

bool HaView::OnClose(bool deleteWindow)
{
    Activate(false);
    auto *frame = static_cast<HaMainFrame *>(GetFrame());
    frame->SetTitle(APP_NAME);
    m_book->DeleteAllPages();
    m_book->Show(false);
    return wxView::OnClose(deleteWindow);
}

void HaView::OnUpdate([[maybe_unused]] wxView *sender, [[maybe_unused]] wxObject *hint)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    GetCurrentHaPanel()->ShowDocument(GetHaDocument());
}

void HaView::OnDraw([[maybe_unused]] wxDC *dc)
{
    // do nothing.
}

void HaView::OnClosingDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
}

void HaView::OnUpdateMenu(wxUpdateUIEvent &event)
{
    Utils::DelegateEvent(GetCurrentHaPanel(), event);
}

void HaView::OnMenu(wxCommandEvent &event)
{
    Utils::DelegateEvent(GetCurrentHaPanel(), event);
}

void HaView::OnImport([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    if (m_importPanel == nullptr) {
        m_importPanel = new ImportPanel(m_book);
        m_book->AddPage(m_importPanel, _("Import"));
    }
    m_book->SetSelection(m_book->FindPage(m_importPanel));
}

void HaView::OnPageChanging(wxBookCtrlEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    int sel = event.GetOldSelection();
    auto *panel = GetHaPanel(sel);
    if (panel != nullptr) {
        panel->HideDocument();
    }
}

void HaView::OnPageChanged(wxBookCtrlEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    int sel = event.GetSelection();
    auto *panel = GetHaPanel(sel);
    if (panel != nullptr) {
        panel->ShowDocument(GetHaDocument());
    }
}

void HaView::SaveContents()
{
    GetCurrentHaPanel()->TrySaveContents();
}

HaDocument *HaView::GetHaDocument() const
{
    return dynamic_cast<HaDocument *>(GetDocument());
}

HaPanel *HaView::GetHaPanel(int sel) const
{
    return dynamic_cast<HaPanel *>(m_book->GetPage(sel));
}

HaPanel *HaView::GetCurrentHaPanel() const
{
    return dynamic_cast<HaPanel *>(m_book->GetCurrentPage());
}
