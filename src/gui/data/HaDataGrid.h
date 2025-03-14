#ifndef _HA_DATA_HA_DATA_GRID_H_
#define _HA_DATA_HA_DATA_GRID_H_

#include "../HaGrid.h"

#include "DataDoc.h"
#include "HaDataTable.h"

class HaDataGrid : public HaGrid
{
    DECLARE_DYNAMIC_CLASS(HaDataGrid)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    HaDataGrid();
    HaDataGrid(
        wxWindow *parent,
        wxWindowID id = wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = wxWANTS_CHARS,
        const wxString &name = wxGridNameStr
    );
    virtual ~HaDataGrid();

    DataDoc *GetTableDoc() override;

private:
    HaTable *CreateHaTable(CsvDoc *doc) override;

    int CursorColOfNewRow() override
    {
        return HaDataTable::OUTLAY_COL;
    }

    HaDataTable *GetHaDataTable() const
    {
        return static_cast<HaDataTable *>(GetTable());
    }
};

#endif /* _HA_DATA_HA_DATA_GRID_H_ */
