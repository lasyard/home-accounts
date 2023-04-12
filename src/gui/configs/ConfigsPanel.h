#ifndef _CONFIGS_CONFIGS_PANEL_H_
#define _CONFIGS_CONFIGS_PANEL_H_

#include <map>

#include "../Common.h"
#include "../HaGrid.h"
#include "../HaPanel.h"

class wxListbook;

class CsvTableBase;

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

private:
    static const wxString ACCOUNTS_LABEL;
    static const wxString ACCOUNTS_COLUMN_LABELS[];
    static const wxString CHANNELS_LABEL;
    static const wxString CHANNELS_COLUMN_LABELS[];

    wxListbook *m_book;
    std::map<wxString, HaGrid *> m_grids;

    void UpdateConfig(const wxString &label, const wxString &name);
    void UpdateGrid(HaGrid *grid);
    void SetGridTable(HaGrid *grid, const wxString &name);
    void SaveGridTable(HaGrid *grid);

    HaGrid *GetCurrentGrid() const
    {
        return static_cast<HaGrid *>(m_book->GetCurrentPage());
    }
};

#endif /* _CONFIGS_CONFIGS_PANEL_H_ */
