#ifndef _HA_IMPORT_IMPORT_GRID_H_
#define _HA_IMPORT_IMPORT_GRID_H_

#include "../HaGridTemplate.h"

#include "ImportDoc.h"
#include "ImportTable.h"

class ImportGrid : public HaGridTemplate<ImportTable, ImportDoc>
{
    DECLARE_DYNAMIC_CLASS(ImportGrid)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM(ImportGrid)

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
};

#endif /* _HA_IMPORT_IMPORT_GRID_H_ */
