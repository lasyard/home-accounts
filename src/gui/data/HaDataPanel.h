#ifndef _HA_PANEL_HA_DATA_PANEL_H_
#define _HA_PANEL_HA_DATA_PANEL_H_

#include "../HaPanel.h"
#include "../Utils.h"

class wxDatePickerCtrl;
class wxDateEvent;
class wxStaticText;
class wxBoxSizer;

class HaDataGrid;

class HaDataPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(HaDataPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    HaDataPanel(wxWindow *parent = nullptr);
    virtual ~HaDataPanel();

    void OnUpdate() override;
    void SaveContents() override;
    void ClearContents() override;

    void OnUpdateMenu(wxUpdateUIEvent &event);
    void OnMenu(wxCommandEvent &event);
    void OnDateChanged(wxDateEvent &event);

    void OnUpdateStatistic(wxCommandEvent &event);

private:
    static const char *const DATA_PREFIX;

    wxBoxSizer *m_header;
    wxDatePickerCtrl *m_date;
    wxStaticText *m_opening;
    wxStaticText *m_closing;
    wxStaticText *m_income;
    wxStaticText *m_outlay;
    HaDataGrid *m_grid;

    std::string m_currentSection;
    bool m_parseError;

    void DoSetDocument(HaDocument *doc) override;

    void ShowDataOfDate(const wxDateTime &date);
    void UpdateStatistic();

    wxStaticText *
    AddLabel(wxSizer *sizer, const wxString &title, const wxFont &font, int borderDirection = wxLEFT | wxRIGHT);
};

#endif /* _HA_PANEL_HA_DATA_PANEL_H_ */
