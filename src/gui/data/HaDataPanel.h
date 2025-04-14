#ifndef _HA_DATA_HA_DATA_PANEL_H_
#define _HA_DATA_HA_DATA_PANEL_H_

#include "../HaPanel.h"
#include "../Utils.h"

#include "csv/money.h"

class wxDatePickerCtrl;
class wxDateEvent;
class wxStaticText;
class wxBoxSizer;

class HaDataGrid;
class CsvDoc;

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

    int m_currentYear;
    int m_currentMonth;
    bool m_parseError;

    CsvDoc *m_period_doc;

    void DoSetDocument(HaDocument *doc) override;

    void ShowDataOfDate(int year, int month);
    void UpdateStatistic();

    const std::string *GetOrCreateSection();
    money_t GetOpening(int year, int month);

    wxStaticText *
    AddLabel(wxSizer *sizer, const wxString &title, const wxFont &font, int borderDirection = wxLEFT | wxRIGHT);

    void FillAnnuallySectionName(char *buf, size_t len);
    void FillMonthlySectionName(char *buf, size_t len, int year);
    void FillDataSectionName(char *buf, size_t len, int year, int month);

    void SetPeriodStat(const char *sectionName, int period, money_t income, money_t outlay);
};

#endif /* _HA_DATA_HA_DATA_PANEL_H_ */
