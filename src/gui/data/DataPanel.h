#ifndef _DATA_DATA_PANEL_H_
#define _DATA_DATA_PANEL_H_

#include <wx/datetime.h>
#include <wx/string.h>

#include "../HaPanel.h"

class wxDateEvent;
class wxDatePickerCtrl;

class DataGrid;

class DataPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(DataPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    static const wxString LABEL;

    DataPanel(wxWindow *parent = nullptr, HaDocument *doc = nullptr);
    virtual ~DataPanel();

    void OnUpdate() override;
    void SaveContents() override;

    void OnDateChanged(wxDateEvent &event);

    void OnUpdateImport(wxUpdateUIEvent &event);
    void OnImport(wxCommandEvent &event);
    void OnUpdateExport(wxUpdateUIEvent &event);
    void OnExport(wxCommandEvent &event);
    void OnUpdatePasteBill(wxUpdateUIEvent &event);
    void OnPasteBill(wxCommandEvent &event);
    void OnUpdateMenu(wxUpdateUIEvent &event);
    void OnMenu(wxCommandEvent &event);
    void OnMenuModify(wxCommandEvent &event);

    void ShowData(const wxDateTime &date);

private:
    static const wxString DATA_FILE_PREFIX;

    wxDatePickerCtrl *m_date;
    DataGrid *m_grid;

    wxString Description();

    static wxString GetSectionName(const wxDateTime &date)
    {
        return DATA_FILE_PREFIX + wxString::Format("/%04d/%02d", date.GetYear(), date.GetMonth() + 1);
    }
};

#endif /* _DATA_DATA_PANEL_H_ */
