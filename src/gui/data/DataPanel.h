#ifndef _DATA_DATA_PANEL_H_
#define _DATA_DATA_PANEL_H_

#include <wx/string.h>

#include "../HaPanel.h"
#include "DataGrid.h"
#include "DataTable.h"

class wxDatePickerCtrl;

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

    void OnUpdateMenu(wxUpdateUIEvent &event);
    void OnMenu(wxCommandEvent &event);
    void OnMenuModify(wxCommandEvent &event);

    void ShowData(const wxString &name);

private:
    static const wxString DATA_FILE_PREFIX;

    wxDatePickerCtrl *m_date;
    DataGrid *m_grid;
};

#endif /* _DATA_DATA_PANEL_H_ */
