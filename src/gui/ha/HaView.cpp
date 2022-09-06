#include "HaView.h"
#include "DataGrid.h"
#include "DataTable.h"
#include "Defs.h"
#include "HaApp.h"
#include "HaDocument.h"
#include "HaMainFrame.h"

IMPLEMENT_DYNAMIC_CLASS(HaView, HaViewBase)
IMPLEMENT_TM(HaView)

BEGIN_EVENT_TABLE(HaView, HaViewBase)
EVT_MENU(ID_CHANGE_PASS, HaViewBase::OnChangePass)
END_EVENT_TABLE()

bool HaView::OnCreate([[maybe_unused]] wxDocument *doc, [[maybe_unused]] long flags)
{
    auto frame = wxGetApp().GetTopWindow();
    SetFrame(frame);
    Activate(true);
    return true;
}

void HaView::OnOpenDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto grid = GetTransactionsGrid();
    ColumnType types[]{INT32, INT64, STR};
    grid->SetTable(new DataTable(types, sizeof(types) / sizeof(ColumnType)));
    grid->Show();
    // Row labels are not updated even by SetTable, so do this.
    grid->ForceRefresh();
    grid->SetAttributes();
}

void HaView::OnClosingDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto grid = GetTransactionsGrid();
    grid->Show(false);
}

void HaView::DeletePages()
{
}

void HaView::SavePages()
{
}

void HaView::DiscardEdits()
{
}

DataGrid *HaView::GetTransactionsGrid() const
{
    return static_cast<HaMainFrame *>(GetFrame())->m_transactionsGrid;
}
