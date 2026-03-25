#ifndef _HA_DATA_DATA_GRID_CELL_ATTR_PROVIDER_H_
#define _HA_DATA_DATA_GRID_CELL_ATTR_PROVIDER_H_

#include "../HaGridCellAttrProvider.h"

class HaTable;

class DataGridCellAttrProvider : public HaGridCellAttrProvider
{
public:
    DECLARE_TM(DataGridCellAttrProvider)

    DataGridCellAttrProvider(HaTable *table);
    virtual ~DataGridCellAttrProvider();

protected:
    wxGridCellAttr *m_deficitAttrRO;

    wxGridCellAttr *GetItemCellAttr(int row, int col) const override;
};

#endif /* _HA_DATA_DATA_GRID_CELL_ATTR_PROVIDER_H_ */
