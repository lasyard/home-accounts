#ifndef _HA_GUI_HA_VIEW_H_
#define _HA_GUI_HA_VIEW_H_

#include <wx/docview.h>

#include "Utils.h"

class wxNotebook;

class HaView : public wxView
{
    DECLARE_DYNAMIC_CLASS(HaView)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    HaView();
    virtual ~HaView();

    bool OnCreate(wxDocument *doc, long flags) override;
    bool OnClose(bool deleteWindow) override;

    void OnUpdate(wxView *sender, wxObject *hint) override;
    void OnDraw(wxDC *dc) override;
    void OnClosingDocument() override;

    void OnUpdateMenu(wxUpdateUIEvent &event);
    void OnMenu(wxCommandEvent &event);
    void OnUpdateRawMode(wxUpdateUIEvent &event);
    void OnRawMode(wxCommandEvent &event);

    void SaveContents();

private:
    wxNotebook *m_book;
};

#endif /* _HA_GUI_HA_VIEW_H_ */
