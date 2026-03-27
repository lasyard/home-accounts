#ifndef _HA_IMPORT_IMPORT_PANEL_H_
#define _HA_IMPORT_IMPORT_PANEL_H_

#include "../HaPanel.h"
#include "../Utils.h"

class ImportGrid;
class wxGridEvent;
class wxCommandEvent;

class ImportPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(ImportPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM(ImportPanel)

    ImportPanel(wxWindow *parent = nullptr);
    virtual ~ImportPanel();

    void OnUpdate() override;
    void SaveContents() override;

private:
    ImportGrid *m_grid;

    void SettingDocument(HaDocument *doc) override;
    void OnGridCellChanged(wxGridEvent &event);
    void OnMerge(wxCommandEvent &event);
};

#endif /* _HA_IMPORT_IMPORT_PANEL_H_ */
