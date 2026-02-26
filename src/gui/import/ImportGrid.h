#ifndef _HA_IMPORT_IMPORT_GRID_H_
#define _HA_IMPORT_IMPORT_GRID_H_

#include "../HaGrid.h"

class ImportGrid : public HaGrid
{
    DECLARE_DYNAMIC_CLASS(ImportGrid)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    ImportGrid();
    ImportGrid(
        wxWindow *parent,
        wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = 0,
        const wxString &name = wxASCII_STR(wxGridNameStr)
    );
    ~ImportGrid();

    HaTable *CreateHaTable(CsvDoc *doc) override;
};

#endif /* _HA_IMPORT_IMPORT_GRID_H_ */
