#ifndef _HA_GUI_HA_GRID_CELL_ATTR_PROVIDER_H_
#define _HA_GUI_HA_GRID_CELL_ATTR_PROVIDER_H_

#include <wx/grid.h>

#include "Utils.h"

class HaTable;

class HaGridCellAttrProvider : public wxGridCellAttrProvider
{
public:
    DECLARE_TM()

    HaGridCellAttrProvider(const HaTable *table);
    virtual ~HaGridCellAttrProvider();

    static void InitAttr();
    static void ReleaseAttr();

    wxGridCellAttr *GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) const override;

protected:
    static wxGridCellAttr *defaultAttr;
    static wxGridCellAttr *monoAttr;
    static wxGridCellAttr *integerAttr;
    static wxGridCellAttr *integerAttrRO;
    static wxGridCellAttr *moneyAttr;
    static wxGridCellAttr *moneyAttrRO;
    static wxGridCellAttr *deficitAttrRO;
    static wxGridCellAttr *boolAttr;

    wxGridCellAttr *m_segmentAttr;

    const HaTable *m_table;

    virtual wxGridCellAttr *GetItemCellAttr(int row, int col) const;
};

#endif /* _HA_GUI_HA_GRID_CELL_ATTR_PROVIDER_H_ */
