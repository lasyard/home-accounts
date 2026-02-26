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

IMPLEMENT_DYNAMIC_CLASS(DataPanel, HaPanel)
IMPLEMENT_TM(DataPanel)

BEGIN_EVENT_TABLE(DataPanel, HaPanel)
EVT_UPDATE_UI(ID_INSERT, DataPanel::OnUpdateMenu)
EVT_MENU(ID_INSERT, DataPanel::OnMenu)
EVT_UPDATE_UI(wxID_DELETE, DataPanel::OnUpdateMenu)
EVT_MENU(wxID_DELETE, DataPanel::OnMenu)
EVT_DATE_CHANGED(ID_DATE, DataPanel::OnDateChanged)
END_EVENT_TABLE()

const char *const DataPanel::DATA_SECTION_NAME_FORMAT = "data/%04d";

DataPanel::DataPanel(wxWindow *parent) : HaPanel(parent)
{
    wxLog::AddTraceMask(TM);
    m_header = new wxBoxSizer(wxHORIZONTAL);
    AddHeaderLabel(_("Select Date:"), HaGdi::BIG_FONT, true);
    m_date = new wxDatePickerCtrl(this, ID_DATE);
    m_date->SetFont(HaGdi::BIG_MONO_FONT);
    m_header->Add(m_date, wxSizerFlags().Border(wxRIGHT, 9).Proportion(0));
    AddHeaderLabel(_("Opening:"), HaGdi::BIG_FONT, true);
    m_opening = AddHeaderLabel(wxEmptyString, HaGdi::BIG_MONO_FONT, false);
    AddHeaderLabel(_("Total Income:"), HaGdi::BIG_FONT, true);
    m_income = AddHeaderLabel(wxEmptyString, HaGdi::BIG_MONO_FONT, false);
    m_income->SetBackgroundColour(HaGdi::SURPLUS_COLOR);
    AddHeaderLabel(_("Total Outlay:"), HaGdi::BIG_FONT, true);
    m_outlay = AddHeaderLabel(wxEmptyString, HaGdi::BIG_MONO_FONT, false);
    m_outlay->SetBackgroundColour(HaGdi::DEFICIT_COLOR);
    AddHeaderLabel(_("Closing:"), HaGdi::BIG_FONT, true);
    m_closing = AddHeaderLabel(wxEmptyString, HaGdi::BIG_MONO_FONT, false);
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
    auto tm = m_date->GetValue();
    ShowDataOfYear(tm.GetYear());
}

void DataPanel::SaveContents()
{
    if (m_error) {
        return;
    }
    m_grid->SaveEditControlValue();
    CsvDoc *doc = m_grid->GetTableDoc();
    wxASSERT(doc != nullptr);
    std::string str;
    doc->Write(str);
    m_doc->SaveOrDeleteSection(sectionNameOfYear(m_currentYear), str);
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
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    auto tm = event.GetDate();
    auto year = tm.GetYear();
    if (year != m_currentYear) {
        SaveContents();
        ShowDataOfYear(year);
    }
    m_grid->MakeDateVisible(tm);
}

void DataPanel::OnUpdateStatistic([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    UpdateStatistic();
    event.Skip(); // vital to allow other handlers to process this event
}

void DataPanel::SettingDocument(HaDocument *doc)
{
    m_grid->Bind(wxEVT_MENU, &DataPanel::OnUpdateStatistic, this, ID_INSERT);
    m_grid->Bind(wxEVT_MENU, &HaDocument::OnChange, doc, ID_INSERT);
    m_grid->Bind(wxEVT_MENU, &DataPanel::OnUpdateStatistic, this, wxID_DELETE);
    m_grid->Bind(wxEVT_MENU, &HaDocument::OnChange, doc, wxID_DELETE);
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED, &DataPanel::OnUpdateStatistic, this);
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, doc);
}

void DataPanel::ShowDataOfYear(int year)
{
    m_currentYear = year;
    auto &data = m_doc->GetOrCreateSection(sectionNameOfYear(year));
    auto *csv = new DataDoc(year);
    m_error = !csv->Read(data);
    m_grid->InitTable(csv);
    UpdateStatistic();
}

void DataPanel::UpdateStatistic()
{
    auto *doc = m_grid->GetTableDoc();
    m_opening->SetLabel(doc->GetStatOpeningString());
    m_income->SetLabel(doc->GetStatIncomeString());
    m_outlay->SetLabel(doc->GetStatOutlayString());
    m_closing->SetLabel(doc->GetStatClosingString());
}

wxStaticText *DataPanel::AddHeaderLabel(const wxString &title, const wxFont &font, bool alignRight)
{
    long align = alignRight ? wxALIGN_RIGHT : wxALIGN_LEFT;
    auto *st = new wxStaticText(this, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, align);
    st->SetFont(font);
    long borderDirection = alignRight ? wxLEFT : wxRIGHT;
    m_header->Add(st, wxSizerFlags().Border(borderDirection, 3).Proportion(1));
    return st;
}
