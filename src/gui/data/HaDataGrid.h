#ifndef _HA_PANEL_HA_DATA_GRID_H_
#define _HA_PANEL_HA_DATA_GRID_H_

#include "../HaGrid.h"

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

private:
    wxGridTableBase *CreateHaTable(CsvDoc *doc) override;
};

#endif /* _HA_PANEL_HA_DATA_GRID_H_ */
