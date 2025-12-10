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
    wxGridCellAttr *m_defaultAttr;
    wxGridCellAttr *m_monoAttr;
    wxGridCellAttr *m_integerAttr;
    wxGridCellAttr *m_integerAttrRO;
    wxGridCellAttr *m_moneyAttr;
    wxGridCellAttr *m_moneyAttrRO;
    wxGridCellAttr *m_deficitAttrRO;
    wxGridCellAttr *m_boolAttr;

    const HaTable *m_table;

    void InitAttr();
    void ReleaseAttr();

    virtual wxGridCellAttr *GetItemCellAttr(int row, int col) const;
};

#endif /* _HA_GUI_HA_GRID_CELL_ATTR_PROVIDER_H_ */
