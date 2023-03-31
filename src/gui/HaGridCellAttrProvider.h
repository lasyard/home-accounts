#ifndef _GUI_HA_GRID_CELL_ATTR_PROVIDER_H_
#define _GUI_HA_GRID_CELL_ATTR_PROVIDER_H_

#include <wx/grid.h>

#include "Common.h"

class HaGridCellAttrProvider : public wxGridCellAttrProvider
{
public:
    DECLARE_TM()

    HaGridCellAttrProvider();
    virtual ~HaGridCellAttrProvider();

protected:
    wxFont m_monoFont;

    mutable wxGridCellAttr *m_defaultAttr;
    mutable wxGridCellAttr *m_readOnlyAttr;
    mutable wxGridCellAttr *m_monoAttr;
    mutable wxGridCellAttr *m_boolAttr;
    mutable wxGridCellAttr *m_moneyAttr;

    /**
     * @brief Set the choices of a `wxGridCellAttr` with choice editor. The attr can be set to readonly initially.
     *
     * @param attr the `wxGridCellAttr`
     * @param choices the choices
     */
    void SetChoices(wxGridCellAttr *&attr, const wxArrayString &choices);
};

#endif /* _GUI_HA_GRID_CELL_ATTR_PROVIDER_H_ */
