#ifndef _HA_RAW_RAW_PANEL_H_
#define _HA_RAW_RAW_PANEL_H_

#include "../HaPanel.h"
#include "../Utils.h"

class wxTextCtrl;
class wxTreebook;

class RawPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(HaPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    RawPanel(wxWindow *parent = nullptr);
    virtual ~RawPanel();

    void OnUpdate() override;
    void SaveContents() override;
    void ClearContents() override;

    void OnUpdateInsert(wxUpdateUIEvent &event);
    void OnInsert(wxCommandEvent &event);
    void OnUpdateDelete(wxUpdateUIEvent &event);
    void OnDelete(wxCommandEvent &event);

private:
    wxTreebook *m_book;

    wxString GetFullName(int sel);
    wxTextCtrl *CreateTextCtrl(wxWindow *parent, const wxString &value);

    size_t InsertOrFindSubPage(int parent, const wxString &text, const wxString &value, bool warnDup = true);

    void FitTreeCtrlAndLayout();
};

#endif /* _HA_RAW_RAW_PANEL_H_ */
