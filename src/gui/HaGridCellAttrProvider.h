#ifndef _HA_GUI_HA_GRID_CELL_ATTR_PROVIDER_H_
#define _HA_GUI_HA_GRID_CELL_ATTR_PROVIDER_H_

#include <wx/grid.h>

#include "Utils.h"

class HaTable;

class HaGridCellAttrProvider : public wxGridCellAttrProvider
{
public:
    DECLARE_TM()

    HaGridCellAttrProvider(HaTable *table);
    virtual ~HaGridCellAttrProvider();

    wxGridCellAttr *GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) const override;

protected:
    wxGridCellAttr *m_defaultAttr;
    wxGridCellAttr *m_defaultAttrRO;
    wxGridCellAttr *m_monoAttr;
    wxGridCellAttr *m_monoAttrRO;
    wxGridCellAttr *m_integerAttr;
    wxGridCellAttr *m_integerAttrRO;
    wxGridCellAttr *m_moneyAttr;
    wxGridCellAttr *m_moneyAttrRO;
    wxGridCellAttr *m_deficitAttrRO;
    wxGridCellAttr *m_boolAttr;
    wxGridCellAttr *m_boolAttrRO;
    wxGridCellAttr *m_dateAttr;
    wxGridCellAttr *m_dateAttrRO;
    wxGridCellAttr *m_commentAttrRO;

    HaTable *m_table;

    static wxGridCellAttr *SelectAttrRO(bool ro, wxGridCellAttr *attrRO, wxGridCellAttr *attr)
    {
        if (ro) {
            attrRO->IncRef();
            return attrRO;
        } else {
            attr->IncRef();
            return attr;
        }
    }

    virtual wxGridCellAttr *GetCommentCellAttr(int row) const;
    virtual wxGridCellAttr *GetItemCellAttr(int row, int col) const;
};

#endif /* _HA_GUI_HA_GRID_CELL_ATTR_PROVIDER_H_ */
