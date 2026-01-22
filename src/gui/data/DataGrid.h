#ifndef _HA_DATA_DATA_GRID_H_
#define _HA_DATA_DATA_GRID_H_

#include "../HaGrid.h"

#include "DataDoc.h"

class DataTable;

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

    DataTable *GetDataTable() const;

    DataDoc *GetTableDoc() override;

protected:
    HaTable *CreateHaTable(CsvDoc *doc) override;

private:
    int CursorColOfNewRow() override;
};

#endif /* _HA_DATA_DATA_GRID_H_ */
