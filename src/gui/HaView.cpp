#include <wx/listbook.h>
#include <wx/notebook.h>

#include "ChangePassDialog.h"
#include "Defs.h"
#include "HaApp.h"
#include "HaDocument.h"
#include "HaMainFrame.h"
#include "HaView.h"
#include "configs/ConfigsPanel.h"
#include "data/DataPanel.h"

IMPLEMENT_DYNAMIC_CLASS(HaView, wxView)
IMPLEMENT_TM(HaView)

BEGIN_EVENT_TABLE(HaView, wxView)
EVT_MENU(ID_CHANGE_PASS, HaView::OnChangePass)
EVT_MENU(ID_INSERT, HaView::OnInsert)
EVT_MENU(ID_DELETE, HaView::OnDelete)
EVT_MENU(ID_RAW_MODE, HaView::OnRawMode)
EVT_NOTEBOOK_PAGE_CHANGED(ID_BOOK, HaView::OnPageChanged)
END_EVENT_TABLE()

HaView::HaView() : wxView(), m_book(nullptr)
{
    wxLog::AddTraceMask(TM);
}

HaView::~HaView()
{
}

bool HaView::OnCreate(wxDocument *doc, [[maybe_unused]] long flags)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto frame = wxGetApp().GetTopWindow();
    SetFrame(frame);
    m_book = XRCCTRL(*frame, "book", wxNotebook);
    auto haDoc = static_cast<HaDocument *>(doc);
    HaPanel::AddToBook<DataPanel>(m_book, haDoc);
    HaPanel::AddToBook<ConfigsPanel>(m_book, haDoc);
    m_book->Show();
    Activate(true);
    return true;
}

bool HaView::OnClose(bool deleteWindow)
{
    Activate(false);
    // Not cleared defaultly, so clear it.
    static_cast<wxFrame *>(GetFrame())->SetTitle("");
    m_book->DeleteAllPages();
    m_book->Show(false);
    return wxView::OnClose(deleteWindow);
}

void HaView::OnUpdate([[maybe_unused]] wxView *sender, [[maybe_unused]] wxObject *hint)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
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

void HaView::OnChangePass([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto doc = static_cast<HaDocument *>(GetDocument());
    ChangePassDialog dlg(nullptr, doc->GetPass());
    if (dlg.ShowModal() == wxID_OK) {
        doc->ChangePass(dlg.GetPass());
    }
}

void HaView::OnInsert(wxCommandEvent &event)
{
    GetCurrentPanel()->OnInsert(event);
}

void HaView::OnDelete(wxCommandEvent &event)
{
    GetCurrentPanel()->OnDelete(event);
}

void HaView::OnRawMode(wxCommandEvent &event)
{
    if (event.IsChecked()) {
        HaPanel::AddToBook<RawPanel>(m_book, static_cast<HaDocument *>(GetDocument()));
        // It is the last one.
        m_book->SetSelection(m_book->GetPageCount() - 1);
    } else {
        m_book->SetSelection(0);
    }
}

void HaView::OnPageChanged(wxBookCtrlEvent &event)
{
    int oldSel = event.GetOldSelection();
    int newSel = event.GetSelection();
    wxLogTrace(TM, "\"%s\" called, from %d to %d.", __WXFUNCTION__, oldSel, newSel);
    if (oldSel >= 0) {
        if (GetPanel(oldSel)->OnLeave()) {
            m_book->DeletePage(oldSel);
        }
    }
    GetPanel(newSel)->OnEnter();
}

void HaView::SaveContents()
{
    GetCurrentPanel()->SaveContents();
}
