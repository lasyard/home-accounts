#ifndef _CONFIGS_CONFIGS_PANEL_H_
#define _CONFIGS_CONFIGS_PANEL_H_

#include <map>

#include "../Common.h"
#include "../HaPanel.h"
#include "ConfigsGrid.h"

class wxListbook;

class ConfigsPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(ConfigsPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    static const wxString LABEL;

    ConfigsPanel(wxWindow *parent = nullptr, HaDocument *doc = nullptr);
    virtual ~ConfigsPanel();

    void OnUpdate() override;
    void SaveContents() override;

    void OnPageChanged(wxBookCtrlEvent &event);

    void OnUpdateMenu(wxUpdateUIEvent &event);
    void OnMenu(wxCommandEvent &event);
    void OnMenuModify(wxCommandEvent &event);
    void OnUpdateExport(wxUpdateUIEvent &event);
    void OnExport(wxCommandEvent &event);

private:
    static const wxString EXPORT_EXT;

    wxListbook *m_book;
    std::map<wxString, ConfigsGrid *> m_grids;

    void UpdateConfig(const wxString &label, const wxString &name);
    void UpdateGrid(ConfigsGrid *grid);
    void SetGridTable(ConfigsGrid *grid, const wxString &name);
    void SaveGridTable(ConfigsGrid *grid);

    ConfigsGrid *GetCurrentGrid() const
    {
        return static_cast<ConfigsGrid *>(m_book->GetCurrentPage());
    }

    ConfigsGrid *GetGrid(int sel) const
    {
        return static_cast<ConfigsGrid *>(m_book->GetPage(sel));
    }
};

#endif /* _CONFIGS_CONFIGS_PANEL_H_ */
