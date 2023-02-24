#ifndef _GUI_CELL_ATTRS_H_
#define _GUI_CELL_ATTRS_H_

#include <wx/grid.h>

class CellAttrs
{
private:
    CellAttrs();
    virtual ~CellAttrs();

    CellAttrs(const CellAttrs &)
    {
    }

    CellAttrs &operator=(const CellAttrs &)
    {
        return *this;
    }

public:
    static CellAttrs &ins()
    {
        static CellAttrs instance;
        return instance;
    }

    const wxFont &GetMonoFont()
    {
        return m_monoFont;
    }

    auto GetDefault()
    {
        m_defaultAttr->IncRef();
        return m_defaultAttr;
    }

    auto GetReadOnly()
    {
        m_readOnlyAttr->IncRef();
        return m_readOnlyAttr;
    }

    auto GetNumber()
    {
        m_numberAttr->IncRef();
        return m_numberAttr;
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

    auto GetOverlapped()
    {
        m_overlappedAttr->IncRef();
        return m_overlappedAttr;
    }

private:
    wxFont m_monoFont;

    wxGridCellFloatEditor *m_floatEditor;

    wxGridCellAttr *m_defaultAttr;
    wxGridCellAttr *m_readOnlyAttr;
    wxGridCellAttr *m_numberAttr;
    wxGridCellAttr *m_moneyAttr;
    wxGridCellAttr *m_timeAttr;
    wxGridCellAttr *m_overlappedAttr;
};

#endif /* _GUI_CELL_ATTRS_H_ */
