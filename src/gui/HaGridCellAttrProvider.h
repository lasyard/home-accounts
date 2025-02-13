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

    wxGridCellAttr *GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) const override;

protected:
    wxFont m_monoFont;

    mutable wxGridCellAttr *m_defaultAttr;
    mutable wxGridCellAttr *m_monoAttr;
    mutable wxGridCellAttr *m_integerAttr;
    mutable wxGridCellAttr *m_boolAttr;
    mutable wxGridCellAttr *m_segmentAttr;

    const HaTable *m_table;

    wxGridCellAttr *GetAttrByColumnType(int col) const;
};

#endif /* _HA_GUI_HA_GRID_CELL_ATTR_PROVIDER_H_ */
