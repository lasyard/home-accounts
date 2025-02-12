#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include <cstdio>
#include <sstream>

#include "HaDataPanel.h"

#include "HaDataGrid.h"

#include "../CsvDoc.h"
#include "../HaDefs.h"
#include "../HaDocument.h"
#include "../StdStreamAccessor.h"

#include "file/Exeptions.h"

IMPLEMENT_DYNAMIC_CLASS(HaDataPanel, HaPanel)
IMPLEMENT_TM(HaDataPanel)

BEGIN_EVENT_TABLE(HaDataPanel, HaPanel)
EVT_UPDATE_UI(ID_INSERT, HaDataPanel::OnUpdateMenu)
EVT_MENU(ID_INSERT, HaDataPanel::OnMenu)
EVT_UPDATE_UI(wxID_DELETE, HaDataPanel::OnUpdateMenu)
EVT_MENU(wxID_DELETE, HaDataPanel::OnMenu)
EVT_DATE_CHANGED(ID_DATE, HaDataPanel::OnDateChanged)
END_EVENT_TABLE()

const char *const HaDataPanel::DATA_PREFIX = "data";

const char *const HaDataPanel::m_labels[] = {
    "id",
    "name",
    "active",
};

const enum column_type HaDataPanel::m_types[] = {
    CT_INT32,
    CT_CSTR,
    CT_BOOL,
};

HaDataPanel::HaDataPanel(wxWindow *parent) : HaPanel(parent), m_currentSection()
{
    wxLog::AddTraceMask(TM);
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    auto *header = new wxBoxSizer(wxHORIZONTAL);
    wxFont promptFont(24, wxFONTFAMILY_SCRIPT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    auto *prompt = new wxStaticText(this, wxID_ANY, _("Select Date: "));
    prompt->SetFont(promptFont);
    header->Add(prompt);
    m_date = new wxDatePickerCtrl(this, ID_DATE);
    wxFont dateFont(24, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    m_date->SetFont(dateFont);
    header->Add(m_date, wxSizerFlags().Border(wxALL, 0).Proportion(0));
    sizer->Add(header, wxSizerFlags().Expand().Border(wxALL, 0).Proportion(0));
    m_grid = new HaDataGrid(this, wxID_ANY);
    sizer->Add(m_grid, wxSizerFlags().Expand().Border(wxALL, 0).Proportion(1));
    SetSizer(sizer);
    m_grid->SetAttributes();
}

HaDataPanel::~HaDataPanel()
{
}

void HaDataPanel::OnUpdate()
{
    ShowDataOfDate(m_date->GetValue());
}

void HaDataPanel::SaveContents()
{
    m_grid->SaveEditControlValue();
    auto *csv = m_grid->GetTableDoc();
    std::ostringstream oss;
    csv->Write(stream_writer, &oss);
    m_doc->SaveSection(m_currentSection, oss.str());
}

void HaDataPanel::ClearContents()
{
    m_grid->ClearGrid();
}

void HaDataPanel::OnUpdateMenu(wxUpdateUIEvent &event)
{
    Utils::DelegateEvent(m_grid, event);
}

void HaDataPanel::OnMenu([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    Utils::DelegateEvent(m_grid, event);
}

void HaDataPanel::OnDateChanged(wxDateEvent &event)
{
    ShowDataOfDate(event.GetDate());
}

void HaDataPanel::DoSetDocument(HaDocument *doc)
{
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, doc);
    m_grid->Bind(wxEVT_MENU, &HaDocument::OnChange, doc, ID_INSERT);
    m_grid->Bind(wxEVT_MENU, &HaDocument::OnChange, doc, wxID_DELETE);
}

void HaDataPanel::ShowDataOfDate(const wxDateTime &date)
{
    char buf[16];
    snprintf(buf, 16, "%4s/%04d/%02d", DATA_PREFIX, date.GetYear(), date.GetMonth() + 1);
    m_currentSection = std::string(buf);
    const std::string *data = nullptr;
    try {
        data = &m_doc->GetSection(m_currentSection);
    } catch (SectionNotFound &e) {
        m_doc->SaveSection(m_currentSection, "");
        data = &m_doc->GetSection(m_currentSection);
    }
    wxASSERT(data != nullptr);
    auto *csv = new CsvDoc(sizeof(m_labels) / sizeof(const char *), m_labels, m_types);
    std::istringstream iss(*data);
    csv->Read(stream_reader, &iss);
    m_grid->InitTable(csv);
}
