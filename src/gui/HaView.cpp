#include <wx/listbook.h>
#include <wx/notebook.h>

#include "HaView.h"

#include "Defs.h"
#include "HaApp.h"
#include "HaDocument.h"
#include "HaMainFrame.h"
#include "HaPanel.h"
#include "PasteBillDialog.h"

#include "configs/ConfigsPanel.h"
#include "data/DataPanel.h"
#include "raw/RawPanel.h"
#include "utils/DaoUtils.h"
#include "utils/GuiUtils.h"

IMPLEMENT_DYNAMIC_CLASS(HaView, wxView)
IMPLEMENT_TM(HaView)

BEGIN_EVENT_TABLE(HaView, wxView)
EVT_NOTEBOOK_PAGE_CHANGED(ID_BOOK, HaView::OnPageChanged)
EVT_UPDATE_UI(ID_IMPORT, HaView::OnUpdateMenu)
EVT_MENU(ID_IMPORT, HaView::OnMenu)
EVT_UPDATE_UI(ID_EXPORT, HaView::OnUpdateMenu)
EVT_MENU(ID_EXPORT, HaView::OnMenu)
EVT_UPDATE_UI(ID_INSERT, HaView::OnUpdateMenu)
EVT_MENU(ID_INSERT, HaView::OnMenu)
EVT_UPDATE_UI(wxID_DELETE, HaView::OnUpdateMenu)
EVT_MENU(wxID_DELETE, HaView::OnMenu)
EVT_UPDATE_UI(ID_PASTE_BILL, HaView::OnUpdatePasteBill)
EVT_MENU(ID_PASTE_BILL, HaView::OnPasteBill)
EVT_UPDATE_UI(ID_RAW_MODE, HaView::OnUpdateRawMode)
EVT_MENU(ID_RAW_MODE, HaView::OnRawMode)
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

void HaView::OnUpdateMenu(wxUpdateUIEvent &event)
{
    auto panel = GetCurrentPanel();
    Utils::DelegateEvent(panel, event);
}

void HaView::OnMenu(wxCommandEvent &event)
{
    auto panel = GetCurrentPanel();
    Utils::DelegateEvent(panel, event);
}

void HaView::OnUpdatePasteBill([[maybe_unused]] wxUpdateUIEvent &event)
{
    event.Enable(GetCurrentPanel()->IsKindOf(CLASSINFO(DataPanel)));
}

void HaView::OnPasteBill([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto doc = GetHaDocument();
    wxArrayString accountChoices;
    auto jointAccount = doc->GetAccountsDao().getJoint<1, 0>();
    auto jointChannel = doc->GetChannelsDao().getJoint<1, 0>();
    PasteBillDialog dlg(nullptr, jointAccount, jointChannel);
    if (dlg.ShowModal() == wxID_OK) {
        wxLogTrace(TM, "title = \"%s\", content =\n%s", dlg.GetBillTitle(), dlg.GetContent());
        // New batch.
        auto &dao = doc->GetBatchesDao();
        dao.append();
        dao.setString(dao.getNumberRows() - 1, 1, dlg.GetBillTitle().ToStdString());
        doc->DoSave(dao);
        // TODO
    }
}

void HaView::OnUpdateRawMode(wxUpdateUIEvent &event)
{
    event.Enable(true);
    auto enable = GetCurrentPanel()->IsKindOf(CLASSINFO(RawPanel));
    event.Check(enable);
}

void HaView::OnRawMode(wxCommandEvent &event)
{
    if (event.IsChecked()) {
        HaPanel::AddToBook<RawPanel>(m_book, GetHaDocument());
        // It is the last one.
        m_book->SetSelection(m_book->GetPageCount() - 1);
    } else {
        m_book->SetSelection(0);
    }
}

void HaView::SaveContents()
{
    GetCurrentPanel()->SaveContents();
}

HaDocument *HaView::GetHaDocument() const
{
    return static_cast<HaDocument *>(GetDocument());
}

HaPanel *HaView::GetCurrentPanel() const
{
    return static_cast<HaPanel *>(m_book->GetCurrentPage());
}

HaPanel *HaView::GetPanel(int sel) const
{
    return static_cast<HaPanel *>(m_book->GetPage(sel));
}
