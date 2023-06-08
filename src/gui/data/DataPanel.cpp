#include <fstream>

#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/filedlg.h>

#include "../Defs.h"
#include "../HaDocument.h"

#include "DataGrid.h"
#include "DataPanel.h"
#include "DataTable.h"
#include "PasteBillDialog.h"

#include "data/DataImportDao.h"

IMPLEMENT_DYNAMIC_CLASS(DataPanel, HaPanel)
IMPLEMENT_TM(DataPanel)

BEGIN_EVENT_TABLE(DataPanel, HaPanel)
EVT_DATE_CHANGED(ID_DATE_DATA, DataPanel::OnDateChanged)
EVT_UPDATE_UI(ID_IMPORT, DataPanel::OnUpdateImport)
EVT_MENU(ID_IMPORT, DataPanel::OnImport)
EVT_UPDATE_UI(ID_EXPORT, DataPanel::OnUpdateExport)
EVT_MENU(ID_EXPORT, DataPanel::OnExport)
EVT_UPDATE_UI(ID_INSERT, DataPanel::OnUpdateMenu)
EVT_MENU(ID_INSERT, DataPanel::OnMenuModify)
EVT_UPDATE_UI(wxID_DELETE, DataPanel::OnUpdateMenu)
EVT_MENU(wxID_DELETE, DataPanel::OnMenuModify)
EVT_UPDATE_UI(ID_PASTE_BILL, DataPanel::OnUpdatePasteBill)
EVT_MENU(ID_PASTE_BILL, DataPanel::OnPasteBill)
END_EVENT_TABLE()

const wxString DataPanel::LABEL = _("Transactions");
const wxString DataPanel::DATA_FILE_PREFIX = "data";

DataPanel::DataPanel(wxWindow *parent, HaDocument *doc) : HaPanel(doc)
{
    wxLog::AddTraceMask(TM);
    wxXmlResource::Get()->LoadPanel(this, parent, "panelData");
    m_date = XRCCTRL(*this, "dateData", wxDatePickerCtrl);
    m_grid = XRCCTRL(*this, "gridData", DataGrid);
    m_grid->Bind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, doc);
    m_grid->SetAttributes();
}

DataPanel::~DataPanel()
{
    // This may be not necessary.
    m_grid->Unbind(wxEVT_GRID_CELL_CHANGED, &HaDocument::OnChange, m_doc);
}

void DataPanel::OnUpdate()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    ShowData(m_date->GetValue());
}

void DataPanel::SaveContents()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    SaveGridTable(m_grid);
}

void DataPanel::OnDateChanged(wxDateEvent &event)
{
    wxLogTrace(TM, "\"%s\" called. date = %s", __WXFUNCTION__, event.GetDate().FormatDate());
    SaveContents();
    ShowData(event.GetDate());
}

void DataPanel::OnUpdateImport(wxUpdateUIEvent &event)
{
    event.Enable(true);
}

void DataPanel::OnImport([[maybe_unused]] wxCommandEvent &event)
{
    // In case of failure, the data will be restored.
    SaveGridTable(m_grid);
    auto desc = Description();
    try {
        if (m_grid->ImportFile(desc)) {
            m_doc->Modify(true);
        }
    } catch (const ParseError &e) {
        // Restore the table data.
        m_doc->TryLoad(*m_grid->GetCachedTable()->GetDao());
        wxLogError("Error occurred when importing \"%s\": \"%s\"", desc, e.what());
    }
}

void DataPanel::OnUpdateExport(wxUpdateUIEvent &event)
{
    event.Enable(!m_grid->GetCachedTable()->GetDao()->isEmpty());
}

void DataPanel::OnExport([[maybe_unused]] wxCommandEvent &event)
{
    SaveGridTable(m_grid);
    m_grid->ExportTable(Description());
}

void DataPanel::OnUpdatePasteBill([[maybe_unused]] wxUpdateUIEvent &event)
{
    event.Enable(true);
}

void DataPanel::OnPasteBill([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    PasteBillDialog dlg(nullptr);
    if (dlg.ShowModal() == wxID_OK) {
        wxLogTrace(TM, "content = \n\"%s\"", dlg.GetContent());
    }
}

void DataPanel::OnUpdateMenu(wxUpdateUIEvent &event)
{
    Common::DelegateEvent(m_grid, event);
}

void DataPanel::OnMenu(wxCommandEvent &event)
{
    Common::DelegateEvent(m_grid, event);
}

void DataPanel::OnMenuModify(wxCommandEvent &event)
{
    if (Common::DelegateEvent(m_grid, event)) {
        m_doc->Modify(true);
    }
}

void DataPanel::ShowData(const wxDateTime &date)
{
    auto name = GetSectionName(date);
    auto &dao = m_doc->GetDataDao();
    dao.setName(name.ToStdString());
    m_doc->TryLoad(dao);
    // TODO: Get the correct balance.
    dao.setInitialBalance(0);
    wxDateTime lastDay = date.GetLastMonthDay();
    int year = lastDay.GetYear();
    int month = lastDay.GetMonth() + 1;
    dao.fillMissingPages(jdn(year, month, 1), jdn(year, month, lastDay.GetDay()));
    auto table = new DataTable(&dao);
    table->UpdateChoicesFromJoints();
    m_grid->SetTable(table, true);
    // Vital, for the original grid cursor may be out of range.
    int cursorRow = m_grid->GetGridCursorRow();
    int maxRow = table->GetRowsCount() - 1;
    if (cursorRow > maxRow) {
        m_grid->SetGridCursor(maxRow, m_grid->GetGridCursorCol());
    }
    m_grid->SetFocus();
    m_grid->RefreshContent();
}

wxString DataPanel::Description()
{
    return m_date->GetValue().Format(_("Transactions of Date %Y-%m"));
}
