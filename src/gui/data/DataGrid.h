#ifndef _HA_DATA_DATA_GRID_H_
#define _HA_DATA_DATA_GRID_H_

#include "../HaGridTemplate.h"

#include "DataDoc.h"
#include "DataTable.h"

class DataGrid : public HaGridTemplate<DataTable, DataDoc>
{
    DECLARE_DYNAMIC_CLASS(DataGrid)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM(DataGrid)

    DataGrid();
    DataGrid(
        wxWindow *parent,
        wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxWANTS_CHARS,
        const wxString &name = wxGridNameStr
    );
    virtual ~DataGrid();

    void MakeDateVisible(const wxDateTime &date);

private:
    int CursorColOfNewRow() override;
};

#endif /* _HA_DATA_DATA_GRID_H_ */
