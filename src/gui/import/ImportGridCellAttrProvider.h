#ifndef _HA_IMPORT_IMPORT_GRID_CELL_ATTR_PROVIDER_H_
#define _HA_IMPORT_IMPORT_GRID_CELL_ATTR_PROVIDER_H_

#include "../HaGridCellAttrProvider.h"

class HaTable;

class ImportGridCellAttrProvider : public HaGridCellAttrProvider
{
public:
    DECLARE_TM(ImportGridCellAttrProvider)

    ImportGridCellAttrProvider(HaTable *table);
    virtual ~ImportGridCellAttrProvider();

protected:
    wxGridCellAttr *m_boldTextAttrRO;

    wxGridCellAttr *GetItemCellAttr(int row, int col) const override;
    wxGridCellAttr *GetOtherCellAttr(int row, int col) const override;
};

#endif /* _HA_IMPORT_IMPORT_GRID_CELL_ATTR_PROVIDER_H_ */
