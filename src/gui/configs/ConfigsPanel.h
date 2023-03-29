#ifndef _CONFIGS_CONFIGS_PANEL_H_
#define _CONFIGS_CONFIGS_PANEL_H_

#include <map>

#include "../HaPanel.h"

class wxListbook;

class CsvTableBase;
class HaGrid;

class ConfigsPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(PanelData)

public:
    static const wxString LABEL;

    ConfigsPanel(wxWindow *parent = nullptr, HaDocument *doc = nullptr);
    virtual ~ConfigsPanel();

    void OnInsert(wxCommandEvent &event) override;
    void OnDelete(wxCommandEvent &event) override;

    void OnUpdate() override;
    void SaveContents() override;

    bool IsInsertEnabled() const override;
    bool IsDeleteEnabled() const override;

private:
    static const wxString OWNERS_LABEL;
    static const wxString OWNERS_COLUMN_LABELS[];
    static const wxString ACCOUNTS_LABEL;
    static const wxString ACCOUNTS_COLUMN_LABELS[];
    static const wxString CHANNELS_LABEL;
    static const wxString CHANNELS_COLUMN_LABELS[];

    wxListbook *m_book;
    std::map<std::string, HaGrid *> m_grids;

    void UpdateConfig(const std::string &sectionName, const wxString &label, CsvTableBase *table);

    HaGrid *GetCurrentGrid() const;
};

#endif /* _CONFIGS_CONFIGS_PANEL_H_ */
