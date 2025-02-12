#ifndef _HA_PANEL_HA_DATA_PANEL_H_
#define _HA_PANEL_HA_DATA_PANEL_H_

#include "../HaPanel.h"
#include "../Utils.h"

#include "csv/column_type.h"

class wxDatePickerCtrl;
class HaDataGrid;
class wxDateEvent;

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

private:
    static const char *const DATA_PREFIX;

    static const char *const m_labels[];
    static const enum column_type m_types[];

    wxDatePickerCtrl *m_date;
    HaDataGrid *m_grid;
    std::string m_currentSection;

    void DoSetDocument(HaDocument *doc) override;

    void ShowDataOfDate(const wxDateTime &date);
};

#endif /* _HA_PANEL_HA_DATA_PANEL_H_ */
