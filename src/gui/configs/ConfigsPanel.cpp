#include <wx/listbook.h>
#include <wx/listctrl.h>

#include "../CsvTable.h"
#include "../HaDocument.h"
#include "../HaGrid.h"
#include "ConfigsGridCellAttrProvider.h"
#include "ConfigsPanel.h"

#define CONFIG_COLUMN_PARA(x) sizeof(x##_COLUMN_LABELS) / sizeof(wxString), x##_COLUMN_LABELS

IMPLEMENT_DYNAMIC_CLASS(ConfigsPanel, HaPanel)

const wxString ConfigsPanel::LABEL = _("Configs");

const wxString ConfigsPanel::OWNERS_LABEL = _("Owners");
const wxString ConfigsPanel::OWNERS_COLUMN_LABELS[] = {
    _("ID"),
    _("Name"),
};
const wxString ConfigsPanel::ACCOUNTS_LABEL = _("Accounts");
const wxString ConfigsPanel::ACCOUNTS_COLUMN_LABELS[] = {
    _("ID"),
    _("Name"),
};
const wxString ConfigsPanel::CHANNELS_LABEL = _("Channels");
const wxString ConfigsPanel::CHANNELS_COLUMN_LABELS[] = {
    _("ID"),
    _("Name"),
};

ConfigsPanel::ConfigsPanel(wxWindow *parent, HaDocument *doc) : HaPanel(doc), m_grids()
{
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

void ConfigsPanel::OnInsert(wxCommandEvent &event)
{
    auto grid = GetCurrentGrid();
    grid->OnInsert(event);
    m_doc->Modify(true);
}

void ConfigsPanel::OnDelete(wxCommandEvent &event)
{
    auto grid = GetCurrentGrid();
    grid->OnDelete(event);
    m_doc->Modify(true);
}

void ConfigsPanel::OnUpdate()
{
    UpdateConfig(
        HaDocument::OWNERS_SECTION_NAME,
        OWNERS_LABEL,
        new CsvTable(CONFIG_COLUMN_PARA(OWNERS), &m_doc->GetOwnersDao())
    );
    UpdateConfig(
        HaDocument::ACCOUNTS_SECTION_NAME,
        ACCOUNTS_LABEL,
        new CsvTable(CONFIG_COLUMN_PARA(ACCOUNTS), &m_doc->GetAccountsDao())
    );
    UpdateConfig(
        HaDocument::CHANNELS_SECTION_NAME,
        CHANNELS_LABEL,
        new CsvTable(CONFIG_COLUMN_PARA(CHANNELS), &m_doc->GetChannelsDao())
    );
}

void ConfigsPanel::SaveContents()
{
    for (const auto &[k, v] : m_grids) {
        v->SaveEditControlValue();
    }
    m_doc->DoSaveOwners();
    m_doc->DoSaveAccounts();
    m_doc->DoSaveChannels();
}

bool ConfigsPanel::IsInsertEnabled() const
{
    // Cannot get focus if the grid is empty.
    return true;
}

bool ConfigsPanel::IsDeleteEnabled() const
{
    auto grid = GetCurrentGrid();
    return grid->HasFocus();
}

void ConfigsPanel::UpdateConfig(const std::string &sectionName, const wxString &label, CsvTableBase *table)
{
    HaGrid *grid;
    if (!m_grids.contains(sectionName)) {
        grid = new HaGrid(m_book);
        grid->Bind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, m_doc);
        grid->SetAttributes();
        m_grids[sectionName] = grid;
        m_book->AddPage(grid, label);
    } else {
        grid = m_grids[sectionName];
    }
    table->SetAttrProvider(new ConfigsGridCellAttrProvider(table));
    grid->SetTable(table);
    // Call this after `CreateGrid` or `SetTable`.
    grid->SetSelectionMode(wxGrid::wxGridSelectionModes::wxGridSelectRows);
    grid->AutoFit();
}

HaGrid *ConfigsPanel::GetCurrentGrid() const
{
    return static_cast<HaGrid *>(m_book->GetCurrentPage());
}
