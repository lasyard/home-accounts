#ifndef _HA_DATA_TABLE_CELL_ATTRS_H_
#define _HA_DATA_TABLE_CELL_ATTRS_H_

#include <wx/grid.h>

class DataTableCellAttrs
{
public:
    DataTableCellAttrs(size_t cols);
    virtual ~DataTableCellAttrs();

    auto GetDefault()
    {
        m_defaultAttr->IncRef();
        return m_defaultAttr;
    }

    auto GetMoney()
    {
        m_moneyAttr->IncRef();
        return m_moneyAttr;
    }

    auto GetTime()
    {
        m_timeAttr->IncRef();
        return m_timeAttr;
    }

    auto GetPage()
    {
        m_pageAttr->IncRef();
        return m_pageAttr;
    }

    auto GetOverlapped()
    {
        m_overlapped->IncRef();
        return m_overlapped;
    }

private:
    wxGridCellFloatEditor *m_floatEditor;

    wxGridCellAttr *m_defaultAttr;
    wxGridCellAttr *m_readOnlyAttr;
    wxGridCellAttr *m_moneyAttr;
    wxGridCellAttr *m_readOnlyMoneyAttr;
    wxGridCellAttr *m_boldReadOnlyMoneyAttr;
    wxGridCellAttr *m_redReadOnlyMoneyAttr;
    wxGridCellAttr *m_timeAttr;
    wxGridCellAttr *m_pageAttr;
    wxGridCellAttr *m_overlapped;
};

#endif /* _HA_DATA_TABLE_CELL_ATTRS_H_ */
