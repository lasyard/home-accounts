#ifndef _HA_ACCOUNT_ACCOUNT_GRID_CELL_ATTR_PROVIDER_H_
#define _HA_ACCOUNT_ACCOUNT_GRID_CELL_ATTR_PROVIDER_H_

#include "../HaGridCellAttrProvider.h"
#include "../Utils.h"

class AccountGridCellAttrProvider : public HaGridCellAttrProvider
{
public:
    DECLARE_TM()

    AccountGridCellAttrProvider(const HaTable *table);
    virtual ~AccountGridCellAttrProvider();

private:
    wxGridCellAttr *GetItemCellAttr(int row, int col) const override;
};

#endif /* _HA_ACCOUNT_ACCOUNT_GRID_CELL_ATTR_PROVIDER_H_ */
