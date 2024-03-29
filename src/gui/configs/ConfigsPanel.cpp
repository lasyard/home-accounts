#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/log.h>

#include "ConfigsPanel.h"

#include "AccountTypesTable.h"
#include "AccountsGridCellAttrProvider.h"
#include "AccountsTable.h"
#include "ConfigsGrid.h"
#include "OwnersTable.h"

#include "../Defs.h"
#include "../HaDocument.h"

#include "../utils/GuiUtils.h"

IMPLEMENT_DYNAMIC_CLASS(ConfigsPanel, HaPanel)
IMPLEMENT_TM(ConfigsPanel)

BEGIN_EVENT_TABLE(ConfigsPanel, HaPanel)
EVT_LISTBOOK_PAGE_CHANGED(ID_BOOK_CONFIGS, ConfigsPanel::OnPageChanged)
EVT_UPDATE_UI(ID_IMPORT, ConfigsPanel::OnUpdateImport)
EVT_MENU(ID_IMPORT, ConfigsPanel::OnImport)
EVT_UPDATE_UI(ID_EXPORT, ConfigsPanel::OnUpdateExport)
EVT_MENU(ID_EXPORT, ConfigsPanel::OnExport)
EVT_UPDATE_UI(ID_INSERT, ConfigsPanel::OnUpdateMenu)
EVT_MENU(ID_INSERT, ConfigsPanel::OnMenuModify)
EVT_UPDATE_UI(wxID_DELETE, ConfigsPanel::OnUpdateMenu)
EVT_MENU(wxID_DELETE, ConfigsPanel::OnMenuModify)
END_EVENT_TABLE()

const wxString ConfigsPanel::LABEL = t("Configs");
const wxString ConfigsPanel::OWNERS_LABEL = t("Owners");
const wxString ConfigsPanel::ACCOUNT_TYPES_LABEL = t("Account Types");
const wxString ConfigsPanel::ACCOUNTS_LABEL = t("Accounts");

ConfigsPanel::ConfigsPanel(wxWindow *parent, HaDocument *doc) : HaPanel(doc), m_grids()
{
    wxLog::AddTraceMask(TM);
    wxXmlResource::Get()->LoadPanel(this, parent, "panelConfigs");
    m_book = XRCCTRL(*this, "bookConfigs", wxListbook);
    // auto list = m_book->GetListView();
    // list->SetMinSize(wxSize(160, -1));
}

ConfigsPanel::~ConfigsPanel()
{
    // This may be not necessary.
    for (const auto &[k, v] : m_grids) {
        v->Unbind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, m_doc);
    }
}

void ConfigsPanel::OnUpdate()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    AddConfig(_(OWNERS_LABEL), new OwnersTable(&m_doc->GetOwnersDao()));
    AddConfig(_(ACCOUNT_TYPES_LABEL), new AccountTypesTable(&m_doc->GetAccountTypesDao()));
    AddConfig(_(ACCOUNTS_LABEL), new AccountsTable(&m_doc->GetAccountsDao()));
    UpdateGrid(GetCurrentGrid());
}

void ConfigsPanel::SaveContents()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    SaveGridTable(GetCurrentGrid());
}

void ConfigsPanel::OnPageChanged(wxBookCtrlEvent &event)
{
    int oldSel = event.GetOldSelection();
    int newSel = event.GetSelection();
    wxLogTrace(TM, "\"%s\" called, from %d to %d.", __WXFUNCTION__, oldSel, newSel);
    if (oldSel >= 0) {
        SaveGridTable(GetGrid(oldSel));
    }
    UpdateGrid(GetGrid(newSel));
}

void ConfigsPanel::OnUpdateImport(wxUpdateUIEvent &event)
{
    event.Enable(GetCurrentGrid() != nullptr);
}

void ConfigsPanel::OnImport([[maybe_unused]] wxCommandEvent &event)
{
    auto sel = m_book->GetSelection();
    if (sel != wxNOT_FOUND) {
        auto grid = GetGrid(sel);
        // In case of failure, the data will be restored.
        SaveGridTable(grid);
        auto desc = Description(sel);
        try {
            if (grid->ImportFile(desc)) {
                m_doc->Modify(true);
            }
        } catch (const ParseError &e) {
            // Restore the table data.
            m_doc->TryLoad(*grid->GetCachedTable()->GetDao());
            wxLogError("Error occurred when importing \"%s\": \"%s\"", desc, e.what());
        }
    }
}

void ConfigsPanel::OnUpdateExport(wxUpdateUIEvent &event)
{
    event.Enable(GetCurrentGrid() != nullptr);
}

void ConfigsPanel::OnExport([[maybe_unused]] wxCommandEvent &event)
{
    auto sel = m_book->GetSelection();
    if (sel != wxNOT_FOUND) {
        auto grid = GetGrid(sel);
        SaveGridTable(grid);
        grid->ExportTable(Description(sel));
    }
}

void ConfigsPanel::OnUpdateMenu(wxUpdateUIEvent &event)
{
    Utils::DelegateEvent(GetCurrentGrid(), event);
}

void ConfigsPanel::OnMenu(wxCommandEvent &event)
{
    Utils::DelegateEvent(GetCurrentGrid(), event);
}

void ConfigsPanel::OnMenuModify(wxCommandEvent &event)
{
    if (Utils::DelegateEvent(GetCurrentGrid(), event)) {
        m_doc->Modify(true);
    }
}

void ConfigsPanel::AddConfig(const wxString &label, CachedTable *table)
{
    auto &name = table->GetDaoName();
    if (!m_grids.contains(name)) {
        auto grid = new ConfigsGrid(m_book);
        grid->Bind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, m_doc);
        grid->SetAttributes();
        // Don't need to detor the table mannually.
        grid->SetTable(table, true, wxGrid::wxGridSelectRows);
        m_grids[name] = grid;
        m_book->AddPage(grid, label);
    }
}

void ConfigsPanel::UpdateGrid(ConfigsGrid *grid)
{
    auto table = grid->GetCachedTable();
    if (table != nullptr) {
        auto accountsTable = dynamic_cast<AccountsTable *>(table);
        if (accountsTable != nullptr) {
            accountsTable->UpdateChoicesFromJoints();
            table->CacheCol(CsvRowTraits<struct account>::OWNER_INDEX);
            table->CacheCol(CsvRowTraits<struct account>::TYPE_INDEX);
        }
        grid->RefreshContent();
    }
}

ConfigsGrid *ConfigsPanel::GetCurrentGrid() const
{
    return static_cast<ConfigsGrid *>(m_book->GetCurrentPage());
}

ConfigsGrid *ConfigsPanel::GetGrid(int sel) const
{
    return static_cast<ConfigsGrid *>(m_book->GetPage(sel));
}

wxString ConfigsPanel::Description(int sel)
{
    return m_book->GetPageText(sel) + _(" Configuration");
}
