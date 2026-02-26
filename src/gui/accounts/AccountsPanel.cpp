#include <wx/log.h>
#include <wx/sizer.h>

#include "AccountsPanel.h"

#include "AccountsDoc.h"
#include "AccountsGrid.h"

#include "../HaDefs.h"
#include "../HaDocument.h"

IMPLEMENT_DYNAMIC_CLASS(AccountsPanel, HaPanel)
IMPLEMENT_TM(AccountsPanel)

BEGIN_EVENT_TABLE(AccountsPanel, HaPanel)
EVT_UPDATE_UI(ID_INSERT, AccountsPanel::OnUpdateMenu)
EVT_MENU(ID_INSERT, AccountsPanel::OnMenu)
EVT_UPDATE_UI(wxID_DELETE, AccountsPanel::OnUpdateMenu)
EVT_MENU(wxID_DELETE, AccountsPanel::OnMenu)
END_EVENT_TABLE()

const char *const AccountsPanel::ACCOUNT_SECTION_NAME = "accounts";

AccountsPanel::AccountsPanel(wxWindow *parent) : HaPanel(parent), m_error(false)
{
    wxLog::AddTraceMask(TM);
    m_grid = Utils::AddSoleGrid<AccountsGrid>(this);
}

AccountsPanel::~AccountsPanel()
{
}

void AccountsPanel::OnUpdate()
{
    auto &data = m_doc->GetOrCreateSection(ACCOUNT_SECTION_NAME);
    auto *csv = new AccountsDoc();
    m_error = !csv->Read(data);
    m_grid->InitTable(csv);
}

void AccountsPanel::SaveContents()
{
    if (m_error) {
        return;
    }
    m_grid->SaveEditControlValue();
    CsvDoc *doc = m_grid->GetTableDoc();
    wxASSERT(doc != nullptr);
    std::string str;
    doc->Write(str);
    m_doc->SaveOrDeleteSection(ACCOUNT_SECTION_NAME, str);
}

void AccountsPanel::ClearContents()
{
    m_grid->ClearGrid();
}

void AccountsPanel::OnUpdateMenu(wxUpdateUIEvent &event)
{
    Utils::DelegateEvent(m_grid, event);
}

void AccountsPanel::OnMenu(wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    Utils::DelegateEvent(m_grid, event);
}

void AccountsPanel::SettingDocument(HaDocument *doc)
{
    m_grid->Bind(wxEVT_MENU, &HaDocument::OnChange, doc, ID_INSERT);
    m_grid->Bind(wxEVT_MENU, &HaDocument::OnChange, doc, wxID_DELETE);
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, doc);
}
