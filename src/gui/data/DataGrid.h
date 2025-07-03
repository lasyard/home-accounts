#ifndef _HA_DATA_DATA_GRID_H_
#define _HA_DATA_DATA_GRID_H_

#include "../HaGrid.h"

#include "DataDoc.h"
#include "DataTable.h"

class DataGrid : public HaGrid
{
    DECLARE_DYNAMIC_CLASS(DataGrid)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

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

    DataDoc *GetTableDoc() override;

private:
    HaTable *CreateHaTable(CsvDoc *doc) override;

    int CursorColOfNewRow() override
    {
        return DataTable::OUTLAY_COL;
    }

    DataTable *GetDataTable() const
    {
        return static_cast<DataTable *>(GetTable());
    }
};

#endif /* _HA_DATA_DATA_GRID_H_ */
