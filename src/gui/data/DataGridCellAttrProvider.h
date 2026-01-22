#ifndef _HA_DATA_DATA_GRID_CELL_ATTR_PROVIDER_H_
#define _HA_DATA_DATA_GRID_CELL_ATTR_PROVIDER_H_

#include "../HaGridCellAttrProvider.h"

class HaTable;

class DataGridCellAttrProvider : public HaGridCellAttrProvider
{
public:
    DECLARE_TM()

    DataGridCellAttrProvider(HaTable *table);
    virtual ~DataGridCellAttrProvider();
};

#endif /* _HA_DATA_DATA_GRID_CELL_ATTR_PROVIDER_H_ */
