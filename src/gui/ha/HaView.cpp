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
END_EVENT_TABLE()

bool HaView::OnCreate([[maybe_unused]] wxDocument *doc, [[maybe_unused]] long flags)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto frame = wxGetApp().GetTopWindow();
    SetFrame(frame);
    m_book = XRCCTRL(*frame, "book", wxNotebook);
    m_transactionsGrid = XRCCTRL(*frame, "transactions", DataGrid);
    m_book->Show();
    Activate(true);
    return true;
}

void HaView::OnUpdate([[maybe_unused]] wxView *sender, [[maybe_unused]] wxObject *hint)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto doc = static_cast<HaDocument *>(GetHaDocument());
    if (doc != nullptr) {
        doc->loadData("test");
        auto table = new DataTable(&doc->getDataFile());
        // `AssignTable` is not existing in earlier version of wxWidgets.
        m_transactionsGrid->SetTable(table, true);
    }
    // Row labels are not updated even by SetTable, so do this.
    m_transactionsGrid->SetAttributes();
    m_transactionsGrid->ForceRefresh();
}

void HaView::SaveContents()
{
}

void HaView::ClearContents()
{
    wxASSERT(m_book != nullptr);
    m_book->Show(false);
}

void HaView::DiscardEdits()
{
}
