#include <cstdio>
#include <sstream>

#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/stattext.h>

#include "DataPanel.h"

#include "DataDoc.h"
#include "DataGrid.h"

#include "../HaDefs.h"
#include "../HaDocument.h"
#include "../HaGdi.h"

#include "data/StdStreamAccessor.h"

IMPLEMENT_DYNAMIC_CLASS(DataPanel, HaPanel)
IMPLEMENT_TM(DataPanel)

BEGIN_EVENT_TABLE(DataPanel, HaPanel)
EVT_UPDATE_UI(ID_INSERT, DataPanel::OnUpdateMenu)
EVT_MENU(ID_INSERT, DataPanel::OnMenu)
EVT_UPDATE_UI(wxID_DELETE, DataPanel::OnUpdateMenu)
EVT_MENU(wxID_DELETE, DataPanel::OnMenu)
EVT_DATE_CHANGED(ID_DATE, DataPanel::OnDateChanged)
END_EVENT_TABLE()

const char *const DataPanel::DATA_PREFIX = "data";

DataPanel::DataPanel(wxWindow *parent)
    : HaPanel(parent)
    , m_currentYear(1970)
    , m_currentMonth(1)
    , m_parseError(false)
    , m_period_doc(new CsvDoc(
          sizeof(period_stat_types) / sizeof(enum column_type),
          period_stat_types,
          sizeof(struct period_stat),
          period_stat_data_get
      ))
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
    m_grid = new DataGrid(this, wxID_ANY);
    sizer->Add(m_grid, wxSizerFlags().Expand().Border(wxALL, 0).Proportion(1));
    SetSizer(sizer);
    m_grid->SetAttributes();
}

DataPanel::~DataPanel()
{
}

void DataPanel::OnUpdate()
{
    auto date = m_date->GetValue();
    m_currentYear = date.GetYear();
    m_currentMonth = date.GetMonth() + 1;
    ShowDataOfDate(m_currentYear, m_currentMonth);
}

void DataPanel::SaveContents()
{
    if (m_parseError) {
        return;
    }
    m_grid->SaveEditControlValue();
    DataDoc *doc = m_grid->GetTableDoc();
    wxASSERT(doc != nullptr);
    std::ostringstream oss;
    doc->Write(stream_writer, &oss);
    char buf[32];
    FillDataSectionName(buf, 32, m_currentYear, m_currentMonth);
    m_doc->SaveOrDeleteSection(buf, oss.str());
    FillMonthlySectionName(buf, 32, m_currentYear);
    auto *stat = doc->GetStat();
    SetPeriodStat(buf, m_currentMonth, stat->income, stat->outlay);
    money_t income, outlay;
    sum_period_stat(m_period_doc->GetSegments(), &income, &outlay);
    FillAnnuallySectionName(buf, 32);
    SetPeriodStat(buf, m_currentYear, income, outlay);
}

void DataPanel::ClearContents()
{
    m_grid->ClearGrid();
}

void DataPanel::OnUpdateMenu(wxUpdateUIEvent &event)
{
    Utils::DelegateEvent(m_grid, event);
}

void DataPanel::OnMenu(wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    Utils::DelegateEvent(m_grid, event);
}

void DataPanel::OnDateChanged(wxDateEvent &event)
{
    SaveContents();
    auto &date = event.GetDate();
    m_currentYear = date.GetYear();
    m_currentMonth = date.GetMonth() + 1;
    ShowDataOfDate(m_currentYear, m_currentMonth);
}

void DataPanel::OnUpdateStatistic([[maybe_unused]] wxCommandEvent &event)
{
    UpdateStatistic();
}

void DataPanel::DoSetDocument(HaDocument *doc)
{
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED, &DataPanel::OnUpdateStatistic, this);
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, doc);
    m_grid->Bind(wxEVT_MENU, &HaDocument::OnChange, doc, ID_INSERT);
    m_grid->Bind(wxEVT_MENU, &HaDocument::OnChange, doc, wxID_DELETE);
}

void DataPanel::ShowDataOfDate(int year, int month)
{
    char buf[32];
    FillDataSectionName(buf, 32, year, month);
    auto &data = m_doc->GetOrCreateSection(buf);
    auto *csv = new DataDoc(year, month);
    m_parseError = !csv->Read(data);
    csv->SetOpening(GetOpening(year, month));
    m_grid->InitTable(csv);
    UpdateStatistic();
}

void DataPanel::UpdateStatistic()
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

money_t DataPanel::GetOpening(int year, int month)
{
    char buf[32];
    FillAnnuallySectionName(buf, 32);
    auto &annually = m_doc->GetOrCreateSection(buf);
    m_period_doc->Read(annually);
    money_t opening = get_period_opening(m_period_doc->GetSegments(), year, 0);
    FillMonthlySectionName(buf, 32, year);
    auto &monthly = m_doc->GetOrCreateSection(buf);
    m_period_doc->Read(monthly);
    opening = get_period_opening(m_period_doc->GetSegments(), month, opening);
    return opening;
}

wxStaticText *DataPanel::AddLabel(wxSizer *sizer, const wxString &title, const wxFont &font, int borderDirection)
{
    auto *label = new wxStaticText(this, wxID_ANY, title);
    label->SetFont(font);
    sizer->Add(label, wxSizerFlags().Border(borderDirection, 9));
    return label;
}

void DataPanel::FillAnnuallySectionName(char *buf, size_t len)
{
    snprintf(buf, len, "%4s", DATA_PREFIX);
}

void DataPanel::FillMonthlySectionName(char *buf, size_t len, int year)
{
    snprintf(buf, len, "%4s/%04d", DATA_PREFIX, year);
}

void DataPanel::FillDataSectionName(char *buf, size_t len, int year, int month)
{
    snprintf(buf, len, "%4s/%04d/%02d", DATA_PREFIX, year, month);
}

void DataPanel::SetPeriodStat(const char *sectionName, int period, money_t income, money_t outlay)
{
    auto &data = m_doc->GetOrCreateSection(sectionName);
    m_period_doc->Read(data);
    set_period_stat(m_period_doc->GetSegments(), period, income, outlay);
    std::ostringstream oss;
    m_period_doc->Write(stream_writer, &oss);
    m_doc->SaveOrDeleteSection(sectionName, oss.str());
}
