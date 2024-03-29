#ifndef _DATA_DATA_GRID_H_
#define _DATA_DATA_GRID_H_

#include "DataTable.h"

#include "../Common.h"
#include "../HaGrid.h"

class DataGrid : public HaGrid
{
    DECLARE_DYNAMIC_CLASS(DataGrid)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    DataGrid();
    virtual ~DataGrid();

    void OnGridSelectCell(wxGridEvent &event);

    void OnUpdateInsert(wxUpdateUIEvent &event);
    void OnInsert(wxCommandEvent &event);
    void OnUpdateDelete(wxUpdateUIEvent &event);
    void OnDelete(wxCommandEvent &event);

    void CreateDataTable(DataDao &dao);
};

#endif /* _DATA_DATA_GRID_H_ */
