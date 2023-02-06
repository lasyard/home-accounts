#include <wx/notebook.h>

#include "DataGrid.h"
#include "DataTable.h"
#include "Defs.h"
#include "HaApp.h"
#include "HaDocument.h"
#include "HaMainFrame.h"
#include "HaView.h"

IMPLEMENT_DYNAMIC_CLASS(HaView, HaViewBase)
IMPLEMENT_TM(HaView)

BEGIN_EVENT_TABLE(HaView, HaViewBase)
EVT_MENU(ID_CHANGE_PASS, HaViewBase::OnChangePass)
EVT_MENU(ID_INSERT, HaView::OnInsert)
EVT_MENU(ID_DELETE, HaView::OnDelete)
END_EVENT_TABLE()

bool HaView::OnCreate(wxDocument *doc, [[maybe_unused]] long flags)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto frame = wxGetApp().GetTopWindow();
    SetFrame(frame);
    m_book = XRCCTRL(*frame, "book", wxNotebook);
    m_transactionsGrid = XRCCTRL(*frame, "transactions", DataGrid);
    m_book->Show();
    m_transactionsGrid->Bind(wxEVT_GRID_EDITOR_HIDDEN, &HaDocumentBase::OnChange, static_cast<HaDocument *>(doc));
    Activate(true);
    return true;
}

void HaView::OnUpdate([[maybe_unused]] wxView *sender, [[maybe_unused]] wxObject *hint)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto doc = static_cast<HaDocument *>(GetDocument());
    doc->LoadData("test");
    auto table = new DataTable(&doc->GetDataFile());
    // `AssignTable` is not existing in earlier version of wxWidgets.
    m_transactionsGrid->SetTable(table, true);
    m_transactionsGrid->AutoFit();
    m_transactionsGrid->SetFocus();
}

void HaView::OnInsert(wxCommandEvent &event)
{
    m_transactionsGrid->OnInsert(event);
    GetDocument()->Modify(true);
}

void HaView::OnDelete(wxCommandEvent &event)
{
    m_transactionsGrid->OnDelete(event);
    GetDocument()->Modify(true);
}

bool HaView::IsInsertEnabled()
{
    return m_book->GetSelection() == 0 && m_transactionsGrid->HasFocus();
}

bool HaView::IsDeleteEnabled()
{
    return m_book->GetSelection() == 0 && m_transactionsGrid->HasFocus() && m_transactionsGrid->IsDeleteEnabled();
}

void HaView::SaveContents()
{
    auto doc = static_cast<HaDocument *>(GetDocument());
    doc->SaveData("test");
}

void HaView::ClearContents()
{
    wxASSERT(m_book != nullptr);
    // Important, or the grid will be drawn before data loaded, cause PANIC.
    m_transactionsGrid->SetTable(nullptr);
    m_book->Show(false);
}

void HaView::DiscardEdits()
{
}
