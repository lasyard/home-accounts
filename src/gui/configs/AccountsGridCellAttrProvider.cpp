#include "AccountsGridCellAttrProvider.h"

#include "data/ConfigPodsTraits.h"

AccountsGridCellAttrProvider::AccountsGridCellAttrProvider(const CachedTable *table)
    : ConfigsGridCellAttrProvider(table)
{
    m_ownerAttr = m_readOnlyAttr->Clone();
    m_typeAttr = m_readOnlyAttr->Clone();
}

AccountsGridCellAttrProvider::~AccountsGridCellAttrProvider()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    m_ownerAttr->DecRef();
    m_typeAttr->DecRef();
}

wxGridCellAttr *AccountsGridCellAttrProvider::GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) const
{
    if (col == CsvRowTraits<struct account>::OWNER_INDEX) {
        m_ownerAttr->IncRef();
        // wxLogTrace(TM, "RefCount of ownerAttr is %d", m_ownerAttr->GetRefCount());
        return m_ownerAttr;
    } else if (col == CsvRowTraits<struct account>::TYPE_INDEX) {
        m_typeAttr->IncRef();
        // wxLogTrace(TM, "RefCount of typeAttr is %d", m_typeAttr->GetRefCount());
        return m_typeAttr;
    }
    return ConfigsGridCellAttrProvider::GetAttr(row, col, kind);
}
