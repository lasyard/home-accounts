#include <sstream>

#include <wx/log.h>
#include <wx/sizer.h>

#include "AccountPanel.h"

#include "AccountDoc.h"
#include "AccountGrid.h"

#include "../HaDefs.h"
#include "../HaDocument.h"

#include "data/StdStreamAccessor.h"

IMPLEMENT_DYNAMIC_CLASS(AccountPanel, HaPanel)
IMPLEMENT_TM(AccountPanel)

BEGIN_EVENT_TABLE(AccountPanel, HaPanel)
EVT_UPDATE_UI(ID_INSERT, AccountPanel::OnUpdateMenu)
EVT_MENU(ID_INSERT, AccountPanel::OnMenu)
EVT_UPDATE_UI(wxID_DELETE, AccountPanel::OnUpdateMenu)
EVT_MENU(wxID_DELETE, AccountPanel::OnMenu)
END_EVENT_TABLE()

const char *const AccountPanel::ACCOUNT_SECTION_NAME = "accounts";

AccountPanel::AccountPanel(wxWindow *parent) : HaPanel(parent), m_parseError(false)
{
    wxLog::AddTraceMask(TM);
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    m_grid = new AccountGrid(this, wxID_ANY);
    sizer->Add(m_grid, wxSizerFlags().Expand().Border(wxALL, 0).Proportion(1));
    SetSizer(sizer);
    m_grid->SetAttributes();
}

AccountPanel::~AccountPanel()
{
}

void AccountPanel::OnUpdate()
{
    auto &data = m_doc->GetOrCreateSection(ACCOUNT_SECTION_NAME);
    auto *csv = new AccountDoc();
    m_parseError = !csv->Read(data);
    m_grid->InitTable(csv);
}

void AccountPanel::SaveContents()
{
    if (m_parseError) {
        return;
    }
    m_grid->SaveEditControlValue();
    CsvDoc *doc = m_grid->GetTableDoc();
    wxASSERT(doc != nullptr);
    std::ostringstream oss;
    doc->Write(stream_writer, &oss);
    m_doc->SaveOrDeleteSection(ACCOUNT_SECTION_NAME, oss.str());
}

void AccountPanel::ClearContents()
{
    m_grid->ClearGrid();
}

void AccountPanel::OnUpdateMenu(wxUpdateUIEvent &event)
{
    Utils::DelegateEvent(m_grid, event);
}

void AccountPanel::OnMenu(wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    Utils::DelegateEvent(m_grid, event);
}

void AccountPanel::DoSetDocument(HaDocument *doc)
{
    m_grid->Bind(wxEVT_MENU, &HaDocument::OnChange, doc, ID_INSERT);
    m_grid->Bind(wxEVT_MENU, &HaDocument::OnChange, doc, wxID_DELETE);
}
