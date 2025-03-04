#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include <cstdio>
#include <sstream>

#include "HaDataPanel.h"

#include "DataDoc.h"
#include "HaDataGrid.h"

#include "../HaDefs.h"
#include "../HaDocument.h"
#include "../HaGdi.h"

#include "data/StdStreamAccessor.h"
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

HaDataPanel::HaDataPanel(wxWindow *parent) : HaPanel(parent), m_currentSection(), m_parseError(false)
{
    wxLog::AddTraceMask(TM);
    m_header = new wxBoxSizer(wxHORIZONTAL);
    AddLabel(m_header, _("Select Date: "), HaGdi::BIG_FONT, wxLEFT);
    m_date = new wxDatePickerCtrl(this, ID_DATE);
    m_date->SetFont(HaGdi::BIG_MONO_FONT);
    m_header->Add(m_date, wxSizerFlags().Border(wxRIGHT, 9).Proportion(0));
    AddLabel(m_header, _("Opening: "), HaGdi::BIG_FONT, wxLEFT);
    m_opening = AddLabel(m_header, wxEmptyString, HaGdi::BIG_MONO_FONT, wxRIGHT);
    AddLabel(m_header, _("Total Income: "), HaGdi::BIG_FONT, wxLEFT);
    m_income = AddLabel(m_header, wxEmptyString, HaGdi::BIG_MONO_FONT, wxRIGHT);
    m_income->SetBackgroundColour(HaGdi::SURPLUS_COLOR);
    AddLabel(m_header, _("Total Outlay: "), HaGdi::BIG_FONT, wxLEFT);
    m_outlay = AddLabel(m_header, wxEmptyString, HaGdi::BIG_MONO_FONT, wxRIGHT);
    m_outlay->SetBackgroundColour(HaGdi::DEFICIT_COLOR);
    AddLabel(m_header, _("Closing: "), HaGdi::BIG_FONT, wxLEFT);
    m_closing = AddLabel(m_header, wxEmptyString, HaGdi::BIG_MONO_FONT, wxRIGHT);
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(m_header, wxSizerFlags().Expand().Border(wxALL, 0).Proportion(0));
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
    if (m_parseError) {
        return;
    }
    std::ostringstream oss;
    m_grid->SaveTable(oss);
    if (!oss.str().empty()) {
        m_doc->SaveSection(m_currentSection, oss.str());
    } else {
        m_doc->DeleteSection(m_currentSection);
    }
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
    SaveContents();
    ShowDataOfDate(event.GetDate());
}

void HaDataPanel::OnUpdateStatistic([[maybe_unused]] wxCommandEvent &event)
{
    UpdateStatistic();
}

void HaDataPanel::DoSetDocument(HaDocument *doc)
{
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED, &HaDataPanel::OnUpdateStatistic, this);
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, doc);
    m_grid->Bind(wxEVT_MENU, &HaDocument::OnChange, doc, ID_INSERT);
    m_grid->Bind(wxEVT_MENU, &HaDocument::OnChange, doc, wxID_DELETE);
}

void HaDataPanel::ShowDataOfDate(const wxDateTime &date)
{
    char buf[30];
    int year = date.GetYear();
    int month = date.GetMonth() + 1;
    snprintf(buf, 30, "%4s/%04d/%02d", DATA_PREFIX, year, month);
    m_currentSection = std::string(buf);
    const std::string *data = nullptr;
    try {
        data = &m_doc->GetSection(m_currentSection);
    } catch ([[maybe_unused]] SectionNotFound &e) {
        m_doc->SaveSection(m_currentSection, "");
        data = &m_doc->GetSection(m_currentSection);
    }
    wxASSERT(data != nullptr);
    auto *csv = new DataDoc(year, month);
    std::istringstream iss(*data);
    m_parseError = !csv->Read(stream_reader, &iss);
    m_grid->InitTable(csv);
    UpdateStatistic();
}

void HaDataPanel::UpdateStatistic()
{
    auto *doc = m_grid->GetTableDoc();
    auto *stat = doc->GetStat();
    m_opening->SetLabel(doc->GetMoneyString(stat->opening));
    m_opening->SetBackgroundColour(stat->opening >= 0 ? HaGdi::SURPLUS_COLOR : HaGdi::DEFICIT_COLOR);
    m_closing->SetLabel(doc->GetMoneyString(stat->closing));
    m_closing->SetBackgroundColour(stat->closing >= 0 ? HaGdi::SURPLUS_COLOR : HaGdi::DEFICIT_COLOR);
    m_income->SetLabel(doc->GetMoneyString(stat->income));
    m_outlay->SetLabel(doc->GetMoneyString(stat->outlay));
    m_header->Layout();
}

wxStaticText *HaDataPanel::AddLabel(wxSizer *sizer, const wxString &title, const wxFont &font, int borderDirection)
{
    auto *label = new wxStaticText(this, wxID_ANY, title);
    label->SetFont(font);
    sizer->Add(label, wxSizerFlags().Border(borderDirection, 9));
    return label;
}
