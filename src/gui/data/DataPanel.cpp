#include <wx/translation.h>

#include "../HaDocument.h"
#include "DataPanel.h"

IMPLEMENT_DYNAMIC_CLASS(DataPanel, HaPanel)

const wxString DataPanel::NAME = "panelData";
const wxString DataPanel::LABEL = _("Transactions");

DataPanel::DataPanel(wxWindow *parent, HaDocument *doc) : HaPanel(doc)
{
    wxXmlResource::Get()->LoadPanel(this, parent, NAME);
    m_dataGrid = XRCCTRL(*this, "gridData", DataGrid);
    m_dataGrid->Bind(wxEVT_GRID_EDITOR_HIDDEN, &HaDocument::OnChange, doc);
    m_dataGrid->SetAttributes();
}

DataPanel::~DataPanel()
{
    m_dataGrid->Unbind(wxEVT_GRID_EDITOR_HIDDEN, &HaDocument::OnChange, m_doc);
}

void DataPanel::OnInsert(wxCommandEvent &event)
{
    m_dataGrid->OnInsert(event);
    m_doc->Modify(true);
}

void DataPanel::OnDelete(wxCommandEvent &event)
{
    m_dataGrid->OnDelete(event);
    m_doc->Modify(true);
}

void DataPanel::OnUpdate()
{
    ShowData("test");
}

void DataPanel::SaveContents()
{
    m_doc->SaveData("test");
}

void DataPanel::ShowData(const wxString &name)
{
    m_doc->LoadData(name);
    auto table = new DataTable(&m_doc->GetDataDao());
    // `AssignTable` is not existing in earlier version of wxWidgets.
    m_dataGrid->SetTable(table, true);
    m_dataGrid->AutoFit();
    m_dataGrid->SetFocus();
}

bool DataPanel::IsInsertEnabled() const
{
    return m_dataGrid->IsInsertEnabled();
}

bool DataPanel::IsDeleteEnabled() const
{
    return m_dataGrid->IsDeleteEnabled();
}
