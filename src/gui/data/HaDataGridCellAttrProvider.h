#ifndef _HA_DATA_HA_DATA_GRID_CELL_ATTR_PROVIDER_H_
#define _HA_DATA_HA_DATA_GRID_CELL_ATTR_PROVIDER_H_

#include <wx/grid.h>

#include "../HaGridCellAttrProvider.h"
#include "../Utils.h"

class HaDataTable;

class HaDataGridCellAttrProvider : public HaGridCellAttrProvider
{
public:
    DECLARE_TM()

    HaDataGridCellAttrProvider(const HaDataTable *table);
    virtual ~HaDataGridCellAttrProvider();

    wxGridCellAttr *GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) const override;

private:
    mutable wxGridCellAttr *m_segmentAttr;
};

#endif /* _HA_DATA_HA_DATA_GRID_CELL_ATTR_PROVIDER_H_ */
