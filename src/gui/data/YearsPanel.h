#ifndef _HA_DATA_YEARS_PANEL_H_
#define _HA_DATA_YEARS_PANEL_H_

#include "../HaPanel.h"
#include "../WxUtils.h"

class YearsGrid;

class YearsPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(YearsPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM(YearsPanel)

    YearsPanel(wxWindow *parent = nullptr);
    virtual ~YearsPanel();

    void OnUpdate() override;

private:
    YearsGrid *m_grid;
};

#endif /* _HA_DATA_YEARS_PANEL_H_ */
