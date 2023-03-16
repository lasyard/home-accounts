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

    auto CloneDefault()
    {
        return m_defaultAttr->Clone();
    }

    auto GetReadOnly()
    {
        m_readOnlyAttr->IncRef();
        return m_readOnlyAttr;
    }

    auto CloneReadOnly()
    {
        return m_readOnlyAttr->Clone();
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

    auto GetBool()
    {
        m_boolAttr->IncRef();
        return m_boolAttr;
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

    wxGridCellAttr *m_defaultAttr;
    wxGridCellAttr *m_readOnlyAttr;
    wxGridCellAttr *m_numberAttr;
    wxGridCellAttr *m_moneyAttr;
    wxGridCellAttr *m_boolAttr;
    wxGridCellAttr *m_timeAttr;
    wxGridCellAttr *m_overlappedAttr;

    void Release();
};

#endif /* _GUI_CELL_ATTRS_H_ */
