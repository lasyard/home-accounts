#ifndef _GUI_HA_VIEW_H_
#define _GUI_HA_VIEW_H_

#include <wx/bookctrl.h>
#include <wx/docview.h>

#include "HaPanel.h"

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

    void OnPageChanged(wxBookCtrlEvent &event);

    void OnUpdateMenu(wxUpdateUIEvent &event);
    void OnMenu(wxCommandEvent &event);
    void OnUpdatePasteBill(wxUpdateUIEvent &event);
    void OnPasteBill(wxCommandEvent &event);
    void OnUpdateRawMode(wxUpdateUIEvent &event);
    void OnRawMode(wxCommandEvent &event);

    void SaveContents();

private:
    wxNotebook *m_book;

    HaPanel *GetCurrentPanel() const
    {
        return static_cast<HaPanel *>(m_book->GetCurrentPage());
    }

    HaPanel *GetPanel(int sel) const
    {
        return static_cast<HaPanel *>(m_book->GetPage(sel));
    }
};

#endif /* _GUI_HA_VIEW_H_ */
