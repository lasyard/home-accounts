#ifndef _HA_IMPORT_IMPORT_PANEL_H_
#define _HA_IMPORT_IMPORT_PANEL_H_

#include "../HaPanel.h"
#include "../Utils.h"

class ImportGrid;
class CsvDoc;

class ImportPanel : public HaPanel
{
    DECLARE_DYNAMIC_CLASS(ImportPanel)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    ImportPanel(wxWindow *parent = nullptr);
    virtual ~ImportPanel();

    void OnUpdate() override;
    void SaveContents() override;

private:
    static const char *const IMPORT_SECTION_NAME;

    ImportGrid *m_grid;

    bool m_error;
};

#endif /* _HA_IMPORT_IMPORT_PANEL_H_ */
