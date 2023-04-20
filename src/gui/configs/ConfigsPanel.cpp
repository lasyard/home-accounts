#include <fstream>

#include <wx/filedlg.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/msgdlg.h>

#include "../Configs.h"
#include "../Defs.h"
#include "../HaDocument.h"
#include "AccountTypesTable.h"
#include "AccountsGridCellAttrProvider.h"
#include "AccountsTable.h"
#include "ChannelsTable.h"
#include "ConfigsPanel.h"
#include "OwnersTable.h"

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

const wxString ConfigsPanel::LABEL = _("Configs");

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
    UpdateConfig(OwnersTable::LABEL, Configs::OWNERS_SECTION_NAME);
    UpdateConfig(AccountTypesTable::LABEL, Configs::ACCOUNT_TYPES_SECTION_NAME);
    UpdateConfig(AccountsTable::LABEL, Configs::ACCOUNTS_SECTION_NAME);
    UpdateConfig(ChannelsTable::LABEL, Configs::CHANNELS_SECTION_NAME);
}

void ConfigsPanel::SaveContents()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    for (const auto &[name, grid] : m_grids) {
        SaveGridTable(grid);
    }
}

void ConfigsPanel::OnPageChanged(wxBookCtrlEvent &event)
{
    int oldSel = event.GetOldSelection();
    int newSel = event.GetSelection();
    wxLogTrace(TM, "\"%s\" called, from %d to %d.", __WXFUNCTION__, oldSel, newSel);
    if (oldSel >= 0) {
        auto grid = static_cast<ConfigsGrid *>(m_book->GetPage(oldSel));
        SaveGridTable(grid);
    }
    auto grid = static_cast<ConfigsGrid *>(m_book->GetPage(newSel));
    UpdateGrid(grid);
}

void ConfigsPanel::OnUpdateImport(wxUpdateUIEvent &event)
{
    event.Enable(GetCurrentGrid() != nullptr);
}

void ConfigsPanel::OnImport([[maybe_unused]] wxCommandEvent &event)
{
    auto sel = m_book->GetSelection();
    if (sel != wxNOT_FOUND) {
        auto answer = wxMessageBox(_("Overwrite the existing configuration?"), _("Confirm"), wxYES_NO | wxCENTER);
        if (answer == wxYES) {
            auto fileName = wxLoadFileSelector(_("CSV file"), "CSV file (*.csv)|*.csv|Text file(*.txt)|*.txt");
            if (!fileName.IsEmpty()) {
                std::ifstream is(fileName.ToStdString());
                auto grid = GetGrid(sel);
                auto csvTable = grid->GetCsvTable();
                if (csvTable != nullptr) {
                    try {
                        // In case of failure, the data will be restored.
                        SaveGridTable(grid);
                        csvTable->GetDao()->read(is);
                        SaveGridTable(grid);
                        UpdateGrid(grid);
                        m_doc->Modify(true);
                    } catch (const std::exception &e) {
                        wxLogError("Error occurred when importing config file \"%s\": \"%s\"", fileName, e.what());
                        // Restore the original data.
                        m_doc->TryLoad(csvTable->GetName(), *csvTable->GetDao());
                    }
                }
            }
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
        auto label = m_book->GetPageText(sel);
        SaveGridTable(grid);
        grid->DumpTable([&label](const wxString &name, const DaoBase *dao) -> void {
            auto nameHint = name;
            nameHint.Replace('/', '_');
            auto realName = wxSaveFileSelector(label + _(" Config"), "CSV File (*.csv)|*.csv", nameHint + ".csv");
            if (!realName.IsEmpty()) {
                std::ofstream os(realName.ToStdString());
                dao->write(os);
            }
        });
    }
}

void ConfigsPanel::OnUpdateMenu(wxUpdateUIEvent &event)
{
    Common::DelegateEvent(GetCurrentGrid(), event);
}

void ConfigsPanel::OnMenu(wxCommandEvent &event)
{
    Common::DelegateEvent(GetCurrentGrid(), event);
}

void ConfigsPanel::OnMenuModify(wxCommandEvent &event)
{
    if (Common::DelegateEvent(GetCurrentGrid(), event)) {
        m_doc->Modify(true);
    }
}

void ConfigsPanel::UpdateConfig(const wxString &label, const wxString &name)
{
    ConfigsGrid *grid;
    if (!m_grids.contains(name)) {
        grid = new ConfigsGrid(m_book);
        grid->Bind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, m_doc);
        grid->SetAttributes();
        m_grids[name] = grid;
        m_book->AddPage(grid, label);
    } else {
        grid = m_grids[name];
    }
    SetGridTable(grid, name);
}

void ConfigsPanel::UpdateGrid(ConfigsGrid *grid)
{
    auto table = dynamic_cast<CsvTableBase *>(grid->GetTable());
    if (table != nullptr) {
        SetGridTable(grid, table->GetName());
    }
}

void ConfigsPanel::SetGridTable(ConfigsGrid *grid, const wxString &name)
{
    CsvTableBase *table = nullptr;
    ConfigsGridCellAttrProvider *attrProvider = nullptr;
    if (name == Configs::OWNERS_SECTION_NAME) {
        table = new OwnersTable(&m_doc->GetOwnersDao());
    } else if (name == Configs::ACCOUNT_TYPES_SECTION_NAME) {
        table = new AccountTypesTable(&m_doc->GetAccountTypesDao());
    } else if (name == Configs::ACCOUNTS_SECTION_NAME) {
        table = new AccountsTable(&m_doc->GetAccountsDao());
        wxArrayString choices;
        auto provider = new AccountsGridCellAttrProvider(table);
        m_doc->GetStringsByCol<struct owner, 1>(m_doc->GetOwnersDao(), choices);
        provider->SetOwnerChoices(choices);
        m_doc->GetStringsByCol<struct account_type, 1>(m_doc->GetAccountTypesDao(), choices);
        provider->SetTypeChoices(choices);
        attrProvider = provider;
    } else if (name == Configs::CHANNELS_SECTION_NAME) {
        table = new ChannelsTable(&m_doc->GetChannelsDao());
    }
    wxASSERT_MSG(table != nullptr, "Table not defined.");
    if (attrProvider == nullptr) {
        attrProvider = new ConfigsGridCellAttrProvider(table);
    }
    table->SetAttrProvider(attrProvider);
    grid->SetTable(table, true, wxGrid::wxGridSelectionModes::wxGridSelectRows);
    grid->AutoSizeAll();
}

void ConfigsPanel::SaveGridTable(ConfigsGrid *grid)
{
    grid->DumpTable([this](const wxString &name, const DaoBase *dao) -> void { m_doc->DoSave(name, *dao); });
}
