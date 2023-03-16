#include <wx/listbook.h>
#include <wx/listctrl.h>

#include "../CsvTable.h"
#include "../HaDocument.h"
#include "../HaGrid.h"
#include "ConfigsPanel.h"

#define CONFIG_COLUMN_PARA(x) sizeof(x##_COLUMN_LABELS) / sizeof(wxString), x##_COLUMN_LABELS

IMPLEMENT_DYNAMIC_CLASS(ConfigsPanel, HaPanel)

const wxString ConfigsPanel::LABEL = _("Configs");

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
    m_book->GetListView()->SetMinSize(wxSize(160, -1));
}

ConfigsPanel::~ConfigsPanel()
{
    for (const auto &[k, v] : m_grids) {
        v->Unbind(wxEVT_GRID_EDITOR_HIDDEN, &HaDocument::OnChange, m_doc);
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

void ConfigsPanel::UpdateConfig(const char *sectionName, const wxString &label, CachedTable *table)
{
    if (!m_grids.contains(sectionName)) {
        auto grid = new HaGrid(m_book);
        grid->Bind(wxEVT_GRID_EDITOR_HIDDEN, &HaDocument::OnChange, m_doc);
        grid->SetAttributes();
        m_grids[sectionName] = grid;
        m_book->AddPage(grid, label);
    }
    m_grids[sectionName]->SetTable(table);
    m_grids[sectionName]->AutoFit();
}

HaGrid *ConfigsPanel::GetCurrentGrid() const
{
    return static_cast<HaGrid *>(m_book->GetCurrentPage());
}
