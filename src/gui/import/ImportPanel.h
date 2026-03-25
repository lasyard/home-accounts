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
    DECLARE_TM(ImportPanel)

    ImportPanel(wxWindow *parent = nullptr);
    virtual ~ImportPanel();

    void OnUpdate() override;
    void SaveContents() override;

private:
    static constexpr char IMPORT_SECTION_NAME[] = "import";
    static constexpr char IMPORT_COL_MAP_SECTION_NAME[] = "conf/import_col_map";

    ImportGrid *m_grid;
};

#endif /* _HA_IMPORT_IMPORT_PANEL_H_ */
