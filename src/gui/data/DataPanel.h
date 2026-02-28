#ifndef _HA_DATA_DATA_PANEL_H_
#define _HA_DATA_DATA_PANEL_H_

#include "../HaPanel.h"
#include "../Utils.h"

class wxDatePickerCtrl;
class wxDateEvent;
class wxStaticText;
class wxBoxSizer;

class CsvDoc;
class DataGrid;

class DataPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(DataPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

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
    static const char *const DATA_SECTION_NAME_FORMAT;

    wxBoxSizer *m_header;
    wxDatePickerCtrl *m_date;
    wxStaticText *m_opening;
    wxStaticText *m_closing;
    wxStaticText *m_income;
    wxStaticText *m_outlay;
    DataGrid *m_grid;

    int m_currentYear;

    void SettingDocument(HaDocument *doc) override;

    const std::string sectionNameOfYear(int year) const
    {
        return w2s(wxString::Format(DATA_SECTION_NAME_FORMAT, year));
    }

    void ShowDataOfYear(int year);

    void UpdateStatistic();

    wxStaticText *AddHeaderLabel(const wxString &title, const wxFont &font, bool alignRight);
};

#endif /* _HA_DATA_DATA_PANEL_H_ */
