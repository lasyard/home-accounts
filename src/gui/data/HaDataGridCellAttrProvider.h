#ifndef _HA_DATA_HA_DATA_GRID_CELL_ATTR_PROVIDER_H_
#define _HA_DATA_HA_DATA_GRID_CELL_ATTR_PROVIDER_H_

#include "../HaGridCellAttrProvider.h"
#include "../Utils.h"

class HaDataTable;

class HaDataGridCellAttrProvider : public HaGridCellAttrProvider
{
public:
    DECLARE_TM()

    HaDataGridCellAttrProvider(const HaTable *table);
    virtual ~HaDataGridCellAttrProvider();

private:
    mutable wxGridCellAttr *m_roMoneyAttr;
    mutable wxGridCellAttr *m_roDeficitAttr;

    wxGridCellAttr *GetItemCellAttr(int row, int col) const override;
};

#endif /* _HA_DATA_HA_DATA_GRID_CELL_ATTR_PROVIDER_H_ */
