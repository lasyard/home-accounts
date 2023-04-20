#include <wx/datectrl.h>

#include "../Defs.h"
#include "../HaDocument.h"
#include "DataPanel.h"

IMPLEMENT_DYNAMIC_CLASS(DataPanel, HaPanel)
IMPLEMENT_TM(DataPanel)

BEGIN_EVENT_TABLE(DataPanel, HaPanel)
EVT_UPDATE_UI(ID_IMPORT, DataPanel::OnUpdateMenu)
EVT_MENU(ID_IMPORT, DataPanel::OnMenuModify)
EVT_UPDATE_UI(ID_EXPORT, DataPanel::OnUpdateMenu)
EVT_MENU(ID_EXPORT, DataPanel::OnMenu)
EVT_UPDATE_UI(ID_INSERT, DataPanel::OnUpdateMenu)
EVT_MENU(ID_INSERT, DataPanel::OnMenuModify)
EVT_UPDATE_UI(wxID_DELETE, DataPanel::OnUpdateMenu)
EVT_MENU(wxID_DELETE, DataPanel::OnMenuModify)
END_EVENT_TABLE()

const wxString DataPanel::LABEL = _("Transactions");
const wxString DataPanel::DATA_FILE_PREFIX = "data/";

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
    auto date = m_date->GetValue();
    ShowData(
        DATA_FILE_PREFIX + wxString::Format("%04d", date.GetYear()) + "/" + wxString::Format("%02d", date.GetMonth())
    );
}

void DataPanel::SaveContents()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    m_grid->SaveEditControlValue();
    // m_doc->DoSaveData("test");
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

void DataPanel::ShowData(const wxString &name)
{
    auto &dao = m_doc->GetDataDao();
    m_doc->TryLoad(name, dao);
    auto table = new DataTable(&dao);
    wxArrayString choices;
    m_doc->GetStringsByCol<struct account, 1>(m_doc->GetAccountsDao(), choices);
    table->SetAccountChoices(choices);
    m_doc->GetStringsByCol<struct channel, 1>(m_doc->GetChannelsDao(), choices);
    table->SetChannelChoices(choices);
    // Vital, for the original grid cursor may be out of range.
    int cursorRow = m_grid->GetGridCursorRow();
    int maxRow = table->GetRowsCount() - 1;
    if (cursorRow > maxRow) {
        m_grid->SetGridCursor(maxRow, m_grid->GetGridCursorCol());
    }
    // `AssignTable` is not existing in earlier version of wxWidgets.
    m_grid->SetTable(table, true);
    m_grid->SetFocus();
    m_grid->AutoSizeAll();
}
