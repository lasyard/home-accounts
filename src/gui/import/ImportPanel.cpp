#include <fstream>
#include <map>
#include <vector>

#include <wx/button.h>
#include <wx/choicdlg.h>
#include <wx/datetime.h>
#include <wx/filedlg.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/sizer.h>

#include "ImportPanel.h"

#include "ImportColMapConf.h"
#include "ImportDoc.h"
#include "ImportGrid.h"

#include "../HaDefs.h"
#include "../HaDocument.h"
#include "../HaView.h"

#include "../accounts/AccountsDoc.h"
#include "../data/DataDoc.h"

#include "file/Exceptions.h"

IMPLEMENT_DYNAMIC_CLASS(ImportPanel, HaPanel)

BEGIN_EVENT_TABLE(ImportPanel, HaPanel)
END_EVENT_TABLE()

ImportPanel::ImportPanel(wxWindow *parent) : HaPanel(parent)
{
    wxLog::AddTraceMask(TM);
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    auto *headerSizer = new wxBoxSizer(wxHORIZONTAL);
    headerSizer->AddStretchSpacer(1);
    auto *btnMerge = new wxButton(this, wxID_ANY, _("Merge"));
    headerSizer->Add(btnMerge, wxSizerFlags().Border(wxALL, 3).Proportion(0));
    sizer->Add(headerSizer, wxSizerFlags().Expand().Border(wxLEFT | wxRIGHT | wxTOP, 0).Proportion(0));
    m_grid = new ImportGrid(this, wxID_ANY);
    m_grid->SetAttributes();
    sizer->Add(m_grid, wxSizerFlags().Expand().Border(wxALL, 0).Proportion(1));
    SetSizer(sizer);
    btnMerge->Bind(wxEVT_BUTTON, &ImportPanel::OnMerge, this);
}

ImportPanel::~ImportPanel()
{
}

void ImportPanel::OnUpdate()
{
    bool ok;
    auto colMap = m_doc->LoadCsvDoc<ImportColMapConf>(IMPORT_COL_MAP_SECTION_NAME, ok);
    if (!ok) {
        wxLogError(_("Invalid import column mapping"));
        return;
    }
    auto *csv = new ImportDoc();
    csv->SetColMap(colMap);
    try {
        auto &data = m_doc->GetSection(IMPORT_SECTION_NAME);
        m_ok = csv->Read(data);
    } catch ([[maybe_unused]] SectionNotFound &e) {
        wxFileDialog dlg(
            nullptr,
            _("Select CSV file to import"),
            wxEmptyString,
            wxEmptyString,
            _("CSV files (*.csv)|*.csv|All files (*.*)|*.*"),
            wxFD_OPEN | wxFD_FILE_MUST_EXIST
        );
        if (dlg.ShowModal() != wxID_OK) {
            delete csv;
            return;
        }
        wxString path = dlg.GetPath();
        std::ifstream ifs(w2s(path));
        if (!ifs.is_open()) {
            wxLogError(_("Failed to open file: %s"), path);
            delete csv;
            return;
        }
        m_ok = csv->ReadStream(ifs);
        // there may be something read, even it is not ok
        std::string str;
        csv->Write(str);
        m_doc->SaveOrDeleteSection(IMPORT_SECTION_NAME, str);
        m_doc->Modify(true);
    }
    m_grid->InitTable(csv);
}

void ImportPanel::SaveContents()
{
    // do nothing, as the contents is read only
}

void ImportPanel::SettingDocument(HaDocument *doc)
{
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED, &ImportPanel::OnGridCellChanged, this);
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, doc);
}

void ImportPanel::OnGridCellChanged(wxGridEvent &event)
{
    if (event.GetRow() == 0) {
        auto *import = dynamic_cast<ImportDoc *>(m_grid->GetTableDoc());
        if (import != nullptr) {
            std::string csv;
            import->GetColMap()->Write(csv);
            m_doc->SaveOrDeleteSection(IMPORT_COL_MAP_SECTION_NAME, csv);
            m_doc->Modify(true);
            OnUpdate();
        }
    }
    event.Skip();
}

void ImportPanel::OnMerge([[maybe_unused]] wxCommandEvent &event)
{
    m_grid->SaveEditControlValue();
    auto *import = dynamic_cast<ImportDoc *>(m_grid->GetTableDoc());

    int dateCsvCol = import->GetCsvCol(DataDoc::DATE_COL);
    if (dateCsvCol == CsvDoc::INVALID_COL) {
        wxMessageBox(_("Cannot import: no DATE."));
        return;
    }

    bool ok;
    auto *accounts = m_doc->LoadCsvDoc<AccountsDoc>(ACCOUNTS_SECTION_NAME, ok);
    if (!ok) {
        wxLogError(_("Invalid accounts"));
        delete accounts;
        return;
    }
    wxArrayString accountNames;
    std::vector<int> accountIds;
    accounts->GetIdAndNames(accountIds, accountNames);
    delete accounts;
    if (accountNames.IsEmpty()) {
        wxMessageBox(_("Cannot import: no account is available."));
        return;
    }

    wxSingleChoiceDialog dlg(this, _("Select an account"), _("Merge"), accountNames);
    if (dlg.ShowModal() != wxID_OK) {
        return;
    }
    int sel = dlg.GetSelection();
    wxASSERT(sel >= 0 && (size_t)sel < accountIds.size());
    int accountId = accountIds[sel];

    std::map<int, std::unique_ptr<DataDoc>> docs;
    for (struct list_item *pos = import->GetRecords()->first; pos != NULL; pos = pos->next) {
        auto *importRecord = get_record(pos);
        auto date = import->GetRecordDate(importRecord);
        if (date == UNKNOWN_DATE) {
            wxMessageBox(_("Cannot import: some records have no DATE."));
            return;
        }
        int year, month, day;
        jdn_split(date, &year, &month, &day);
        DataDoc *dataDoc = nullptr;
        if (docs.contains(year)) {
            dataDoc = docs.at(year).get();
        } else {
            bool ok;
            dataDoc = m_doc->LoadDataDoc(year, ok);
            if (!ok) {
                wxLogError(_("Invalid data of year %d"), year);
                return;
            }
            docs[year] = std::unique_ptr<DataDoc>(dataDoc);
        }
        auto time = import->GetRecordTime(importRecord);
        auto *record = dataDoc->InsertRecordAtTime(date, time);
        if (record == nullptr) {
            wxLogError(_("Failed to insert record to data of year %d"), year);
            return;
        }
        dataDoc->SetRecordAccount(record, accountId);
        for (int col = 0; col < DataDoc::COLS; ++col) {
            int csvCol = import->GetCsvCol(col);
            if (csvCol == CsvDoc::INVALID_COL) {
                continue;
            }
            dataDoc->SetRecordField(record, col, import->GetRecordField(importRecord, csvCol));
        }
    }

    for (const auto &[year, dataDoc] : docs) {
        std::string out;
        dataDoc->Write(out);
        m_doc->SaveOrDeleteSection(DataSectionNameOfYear(year), out);
    }

    m_doc->DeleteSection(IMPORT_SECTION_NAME);
    m_doc->Modify(true);
    wxMessageBox(_("Merge completed."));
    m_doc->GetHaView()->CloseImportPanel();
}
