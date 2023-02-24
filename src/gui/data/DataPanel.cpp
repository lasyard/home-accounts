#include <wx/translation.h>

#include "../HaDocument.h"
#include "DataPanel.h"

IMPLEMENT_DYNAMIC_CLASS(DataPanel, HaPanel)

const wxString DataPanel::LABEL = _("Transactions");

DataPanel::DataPanel(wxWindow *parent, HaDocument *doc) : HaPanel(doc)
{
    wxXmlResource::Get()->LoadPanel(this, parent, "panelData");
    m_grid = XRCCTRL(*this, "gridData", DataGrid);
    m_grid->Bind(wxEVT_GRID_EDITOR_HIDDEN, &HaDocument::OnChange, doc);
    m_grid->SetAttributes();
}

DataPanel::~DataPanel()
{
    m_grid->Unbind(wxEVT_GRID_EDITOR_HIDDEN, &HaDocument::OnChange, m_doc);
}

void DataPanel::OnInsert(wxCommandEvent &event)
{
    m_grid->OnInsert(event);
    m_doc->Modify(true);
}

void DataPanel::OnDelete(wxCommandEvent &event)
{
    m_grid->OnDelete(event);
    m_doc->Modify(true);
}

void DataPanel::OnUpdate()
{
    ShowData("test");
}

void DataPanel::SaveContents()
{
    m_doc->DoSave("test", m_doc->GetDataDao());
}

void DataPanel::ShowData(const wxString &name)
{
    auto &dao = m_doc->GetDataDao();
    m_doc->TryLoad(name, dao);
    auto table = new DataTable(&dao);
    // `AssignTable` is not existing in earlier version of wxWidgets.
    m_grid->SetTable(table, true);
    m_grid->AutoFit();
    m_grid->SetFocus();
}

bool DataPanel::IsInsertEnabled() const
{
    return m_grid->HasFocus() && m_grid->IsInsertEnabled();
}

bool DataPanel::IsDeleteEnabled() const
{
    return m_grid->HasFocus() && m_grid->IsDeleteEnabled();
}
