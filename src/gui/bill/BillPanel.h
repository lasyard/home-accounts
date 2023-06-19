#ifndef _BILL_BILL_PANEL_H_
#define _BILL_BILL_PANEL_H_

#include <wx/string.h>

#include "../HaPanel.h"

class wxChoice;

template <typename T, typename S> class Joint;

class DataGrid;

class BillPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(BillPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    static const wxString LABEL;

    BillPanel(wxWindow *parent = nullptr, HaDocument *doc = nullptr);
    virtual ~BillPanel();

    void OnUpdate() override;
    void SaveContents() override;

    void OnChoiceBill(wxCommandEvent &event);

    void OnUpdateExport(wxUpdateUIEvent &event);
    void OnExport(wxCommandEvent &event);
    void OnUpdatePasteBill(wxUpdateUIEvent &event);
    void OnPasteBill(wxCommandEvent &event);
    void OnUpdateMenu(wxUpdateUIEvent &event);
    void OnMenu(wxCommandEvent &event);
    void OnMenuModify(wxCommandEvent &event);

    void ShowData(int batch);

private:
    Joint<const char *, int32_t> *m_batchJoint;

    wxChoice *m_choice;
    DataGrid *m_grid;

    wxString Description();
};

#endif /* _BILL_BILL_PANEL_H_ */
