#ifndef _CONFIGS_CONFIGS_PANEL_H_
#define _CONFIGS_CONFIGS_PANEL_H_

#include <map>

#include "../Common.h"
#include "../HaPanel.h"

class wxListbook;

class ConfigsGrid;

class ConfigsPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(ConfigsPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    static const wxString LABEL;
    static const wxString OWNERS_LABEL;
    static const wxString CHANNELS_LABEL;
    static const wxString ACCOUNT_TYPES_LABEL;
    static const wxString ACCOUNTS_LABEL;

    ConfigsPanel(wxWindow *parent = nullptr, HaDocument *doc = nullptr);
    virtual ~ConfigsPanel();

    void OnUpdate() override;
    void SaveContents() override;

    void OnPageChanged(wxBookCtrlEvent &event);

    void OnUpdateImport(wxUpdateUIEvent &event);
    void OnImport(wxCommandEvent &event);
    void OnUpdateExport(wxUpdateUIEvent &event);
    void OnExport(wxCommandEvent &event);
    void OnUpdateMenu(wxUpdateUIEvent &event);
    void OnMenu(wxCommandEvent &event);
    void OnMenuModify(wxCommandEvent &event);

private:
    wxListbook *m_book;
    std::map<wxString, ConfigsGrid *> m_grids;

    void AddConfig(const wxString &label, CachedTable *table);
    void UpdateGrid(ConfigsGrid *grid);

    ConfigsGrid *GetCurrentGrid() const;
    ConfigsGrid *GetGrid(int sel) const;

    wxString Description(int sel);
};

#endif /* _CONFIGS_CONFIGS_PANEL_H_ */
