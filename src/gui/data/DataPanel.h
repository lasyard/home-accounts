#ifndef _HA_DATA_DATA_PANEL_H_
#define _HA_DATA_DATA_PANEL_H_

#include "../HaPanel.h"
#include "../WxUtils.h"

class wxDatePickerCtrl;
class wxDateEvent;
class wxStaticText;
class wxBoxSizer;

class DataGrid;

class DataPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(DataPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM(DataPanel)

    DataPanel(wxWindow *parent = nullptr);
    virtual ~DataPanel();

    void OnUpdate() override;
    void SaveContents() override;
    void ClearContents() override;

    void OnUpdateMenu(wxUpdateUIEvent &event);
    void OnMenu(wxCommandEvent &event);
    void OnDateChanged(wxDateEvent &event);

    void OnUpdateStatistic(wxCommandEvent &event);

private:
    wxBoxSizer *m_header;
    wxDatePickerCtrl *m_date;
    wxStaticText *m_opening;
    wxStaticText *m_closing;
    wxStaticText *m_income;
    wxStaticText *m_outlay;
    DataGrid *m_grid;

    int m_currentYear;

    void SettingDocument(HaDocument *doc) override;

    void ShowDataOfYear(int year);

    void UpdateStatistic();

    wxStaticText *AddHeaderLabel(const wxString &title, const wxFont &font, bool alignRight);
};

#endif /* _HA_DATA_DATA_PANEL_H_ */
