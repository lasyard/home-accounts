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

#include "data/item.h"

IMPLEMENT_DYNAMIC_CLASS(BillPanel, HaPanel)
IMPLEMENT_TM(BillPanel)

BEGIN_EVENT_TABLE(BillPanel, HaPanel)
EVT_CHOICE(ID_CHOICE_BILL_ACCOUNT, BillPanel::OnChoiceBillAccount)
EVT_CHOICE(ID_CHOICE_BILL_TITLE, BillPanel::OnChoiceBillTitle)
EVT_UPDATE_UI(ID_EXPORT, BillPanel::OnUpdateExport)
EVT_MENU(ID_EXPORT, BillPanel::OnExport)
EVT_UPDATE_UI(ID_INSERT, BillPanel::OnUpdateMenu)
EVT_MENU(ID_INSERT, BillPanel::OnMenuModify)
EVT_UPDATE_UI(wxID_DELETE, BillPanel::OnUpdateMenu)
EVT_MENU(wxID_DELETE, BillPanel::OnMenuModify)
EVT_UPDATE_UI(ID_PASTE_BILL, BillPanel::OnUpdatePasteBill)
EVT_MENU(ID_PASTE_BILL, BillPanel::OnPasteBill)
EVT_UPDATE_UI(ID_BTN_PASTE_BILL, BillPanel::OnUpdatePasteBill)
EVT_BUTTON(ID_BTN_PASTE_BILL, BillPanel::OnPasteBill)
EVT_UPDATE_UI(ID_MERGE, BillPanel::OnUpdateMerge)
EVT_MENU(ID_MERGE, BillPanel::OnMerge)
EVT_UPDATE_UI(ID_BTN_MERGE, BillPanel::OnUpdateMerge)
EVT_BUTTON(ID_BTN_MERGE, BillPanel::OnMerge)
END_EVENT_TABLE()

const wxString BillPanel::LABEL = _("Bills");

BillPanel::BillPanel(wxWindow *parent, HaDocument *doc) : HaPanel(doc)
{
    wxLog::AddTraceMask(TM);
    wxXmlResource::Get()->LoadPanel(this, parent, "panelBill");
    m_choiceAccount = XRCCTRL(*this, "choiceBillAccount", wxChoice);
    m_choiceTitle = XRCCTRL(*this, "choiceBillTitle", wxChoice);
    m_grid = XRCCTRL(*this, "gridBill", DataGrid);
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, doc);
    m_grid->SetAttributes();
}

BillPanel::~BillPanel()
{
    // This may be not necessary.
    m_grid->Unbind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, m_doc);
}

void BillPanel::OnUpdate()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    m_choiceAccount->Clear();
    m_doc->GetAccountsDao().forEach([this](struct account *i) -> bool {
        this->m_choiceAccount->Append(c(i->name), new IntClientData(i->id));
        return true;
    });
    if (!m_choiceAccount->IsEmpty()) {
        Utils::SetChoiceSelection(m_choiceAccount, 0);
    }
}

void BillPanel::SaveContents()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    SaveGridTable(m_grid);
}

void BillPanel::OnChoiceBillAccount(wxCommandEvent &event)
{
    if (event.GetInt() != wxNOT_FOUND) {
        IntClientData *clientData = dynamic_cast<IntClientData *>(event.GetClientObject());
        if (clientData != nullptr) {
            auto account = clientData->get();
            wxLogTrace(TM, "\"%s\" called, selected account id \"%d\".", __WXFUNCTION__, account);
            m_choiceTitle->Clear();
            m_doc->GetBatchesDao().forEach([this, account](struct batch *i) -> bool {
                if (i->account == account) {
                    this->m_choiceTitle->Append(c(i->title), new IntClientData(i->id));
                }
                return true;
            });
            if (!m_choiceTitle->IsEmpty()) {
                Utils::SetChoiceSelection(m_choiceTitle, 0);
            } else {
                Utils::SetChoiceSelection(m_choiceTitle, wxNOT_FOUND);
            }
        }
    }
}

void BillPanel::OnChoiceBillTitle(wxCommandEvent &event)
{
    if (event.GetInt() != wxNOT_FOUND) {
        IntClientData *clientData = dynamic_cast<IntClientData *>(event.GetClientObject());
        if (clientData != nullptr) {
            auto batch = clientData->get();
            wxLogTrace(TM, "\"%s\" called, selected batch id \"%d\".", __WXFUNCTION__, batch);
            ShowData(batch);
        }
    } else {
        wxLogTrace(TM, "\"%s\" called, no batch selected.", __WXFUNCTION__);
        m_grid->SetTable(new wxGridStringTable(0, 0), true);
        m_grid->Refresh();
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

void BillPanel::OnUpdatePasteBill(wxUpdateUIEvent &event)
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
            // TODO: should select the new added bill.
            OnUpdate();
        }
    }
}

void BillPanel::OnUpdateMerge(wxUpdateUIEvent &event)
{
    event.Enable(m_choiceTitle->GetSelection() != wxNOT_FOUND);
}

void BillPanel::OnMerge([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    SaveGridTable(m_grid);
    DataDao dao;
    m_doc->GetBillDao().forEach([this, &dao](date_t date, struct item *item) -> bool {
        auto name = HaDocument::DataSectionName(date);
        if (name != dao.getName()) {
            if (!dao.getName().empty()) {
                this->m_doc->DoSave(dao);
            }
            dao.setName(name);
            this->m_doc->TryLoad(dao);
        }
        auto item1 = add_item_to_date(&dao.getData(), date);
        if (item1 != NULL) {
            copy_item(item1, item);
        }
        return true;
    });
    if (!dao.getName().empty()) {
        this->m_doc->DoSave(dao);
    }
    m_doc->DeleteSection(m_doc->GetBillDao().getName());
    // Refresh to prevent saving back this section.
    OnUpdate();
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
    return m_choiceAccount->GetStringSelection() + m_choiceTitle->GetStringSelection();
}
