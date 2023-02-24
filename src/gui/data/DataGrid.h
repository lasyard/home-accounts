#ifndef _DATA_DATA_GRID_H_
#define _DATA_DATA_GRID_H_

#include <wx/pen.h>

#include "../Common.h"
#include "../HaGrid.h"
#include "DataTable.h"

class DataGrid : public HaGrid
{
    DECLARE_DYNAMIC_CLASS(DataGrid)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    DataGrid();
    virtual ~DataGrid();

    void OnGridSelectCell(wxGridEvent &event);
    void OnInsert(wxCommandEvent &event);
    void OnDelete(wxCommandEvent &event);

    bool IsInsertEnabled() const;
    bool IsDeleteEnabled() const;
};

#endif /* _DATA_DATA_GRID_H_ */
