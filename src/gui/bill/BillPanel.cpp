#include <fstream>

#include <wx/choice.h>
#include <wx/log.h>

#include "BillPanel.h"
#include "PasteBillDialog.h"

#include "../Defs.h"
#include "../HaDocument.h"

#include "../data/DataGrid.h"
#include "../data/DataTable.h"
#include "../utils/DaoUtils.h"
#include "../utils/GuiUtils.h"
#include "../utils/IntClientData.h"

#include "data/CxxDefs.h"

IMPLEMENT_DYNAMIC_CLASS(BillPanel, HaPanel)
IMPLEMENT_TM(BillPanel)

BEGIN_EVENT_TABLE(BillPanel, HaPanel)
EVT_CHOICE(ID_CHOICE_BILL, BillPanel::OnChoiceBill)
EVT_UPDATE_UI(ID_EXPORT, BillPanel::OnUpdateExport)
EVT_MENU(ID_EXPORT, BillPanel::OnExport)
EVT_UPDATE_UI(ID_INSERT, BillPanel::OnUpdateMenu)
EVT_MENU(ID_INSERT, BillPanel::OnMenuModify)
EVT_UPDATE_UI(wxID_DELETE, BillPanel::OnUpdateMenu)
EVT_MENU(wxID_DELETE, BillPanel::OnMenuModify)
EVT_UPDATE_UI(ID_PASTE_BILL, BillPanel::OnUpdatePasteBill)
EVT_MENU(ID_PASTE_BILL, BillPanel::OnPasteBill)
EVT_BUTTON(ID_BTN_PASTE_BILL, BillPanel::OnPasteBill)
END_EVENT_TABLE()

const wxString BillPanel::LABEL = _("Bills");

BillPanel::BillPanel(wxWindow *parent, HaDocument *doc) : HaPanel(doc), m_choice(nullptr)
{
    wxLog::AddTraceMask(TM);
    wxXmlResource::Get()->LoadPanel(this, parent, "panelBill");
    m_batchJoint = doc->GetBatchesDao().getJoint<1, 0>();
    m_choice = XRCCTRL(*this, "choiceBill", wxChoice);
    m_grid = XRCCTRL(*this, "gridBill", DataGrid);
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, doc);
    m_grid->SetAttributes();
}

BillPanel::~BillPanel()
{
    // This may be not necessary.
    m_grid->Unbind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, m_doc);
    delete m_batchJoint;
}

void BillPanel::OnUpdate()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    m_doc->TryLoad(m_doc->GetBatchesDao());
    Utils::SetChoiceItemsWithIds(m_choice, m_batchJoint, false);
    if (!m_choice->IsEmpty()) {
        m_choice->SetSelection(m_choice->GetCount() - 1);
        IntClientData *clientData =
            dynamic_cast<IntClientData *>(m_choice->GetClientObject(m_choice->GetCurrentSelection()));
        if (clientData != nullptr) {
            ShowData(clientData->get());
        }
    }
}

void BillPanel::SaveContents()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    SaveGridTable(m_grid);
}

void BillPanel::OnChoiceBill(wxCommandEvent &event)
{
    IntClientData *clientData = dynamic_cast<IntClientData *>(event.GetClientObject());
    if (clientData != nullptr) {
        auto batch = clientData->get();
        wxLogTrace(TM, "\"%s\" called, selected batch id \"%d\".", __WXFUNCTION__, batch);
        ShowData(batch);
    }
}

void BillPanel::OnUpdateExport(wxUpdateUIEvent &event)
{
    auto table = m_grid->GetCachedTable();
    event.Enable(table != nullptr && !table->GetDao()->isEmpty());
}

void BillPanel::OnExport([[maybe_unused]] wxCommandEvent &event)
{
    SaveGridTable(m_grid);
    m_grid->ExportTable(Description());
}

void BillPanel::OnUpdatePasteBill([[maybe_unused]] wxUpdateUIEvent &event)
{
    event.Enable(true);
}

void BillPanel::OnPasteBill([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    PasteBillDialog dlg(nullptr, &m_doc->GetAccountsDao());
    if (dlg.ShowModal() == wxID_OK) {
        wxLogTrace(TM, "title = \"%s\", content =\n%s", dlg.GetBillTitle(), dlg.GetContent());
        if (m_doc->CreateBill(dlg.GetBillTitle(), dlg.GetContent(), dlg.GetAccount())) {
            OnUpdate();
        }
    }
}

void BillPanel::OnUpdateMenu(wxUpdateUIEvent &event)
{
    Utils::DelegateEvent(m_grid, event);
}

void BillPanel::OnMenu(wxCommandEvent &event)
{
    Utils::DelegateEvent(m_grid, event);
}

void BillPanel::OnMenuModify(wxCommandEvent &event)
{
    if (Utils::DelegateEvent(m_grid, event)) {
        m_doc->Modify(true);
    }
}

void BillPanel::ShowData(int batch)
{
    m_doc->TryLoadBill(batch);
    // TODO: Get the correct balance.
    auto &dao = m_doc->GetBillDao();
    dao.setInitialBalance(0);
    m_grid->CreateDataTable(dao);
}

wxString BillPanel::Description()
{
    return m_choice->GetStringSelection() + _("Bill");
}
