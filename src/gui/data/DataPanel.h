#ifndef _DATA_DATA_PANEL_H_
#define _DATA_DATA_PANEL_H_

#include <wx/string.h>

#include "../HaPanel.h"
#include "DataGrid.h"
#include "DataTable.h"

class DataPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(DataPanel)

public:
    DECLARE_TM()

    static const wxString LABEL;

    DataPanel(wxWindow *parent = nullptr, HaDocument *doc = nullptr);
    virtual ~DataPanel();

    void OnInsert(wxCommandEvent &event) override;
    void OnDelete(wxCommandEvent &event) override;

    void OnUpdate() override;
    void SaveContents() override;

    bool IsInsertEnabled() const override;
    bool IsDeleteEnabled() const override;

    void ShowData(const wxString &name);

private:
    DataGrid *m_grid;
};

#endif /* _DATA_DATA_PANEL_H_ */
