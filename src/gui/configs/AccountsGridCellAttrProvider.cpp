#include "AccountsGridCellAttrProvider.h"
#include "data/ConfigPodsTraits.h"

AccountsGridCellAttrProvider::AccountsGridCellAttrProvider(const CsvTableBase *table)
    : ConfigsGridCellAttrProvider(table)
{
    m_ownerAttr = m_readOnlyAttr->Clone();
}

AccountsGridCellAttrProvider::~AccountsGridCellAttrProvider()
{
    m_ownerAttr->DecRef();
}

wxGridCellAttr *AccountsGridCellAttrProvider::GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) const
{
    if (col == CsvRowTraits<struct account>::OWNER_INDEX) {
        m_ownerAttr->IncRef();
        // wxLogTrace(TM, "RefCount of ownerAttr is %d", m_ownerAttr->GetRefCount());
        return m_ownerAttr;
    }
    return ConfigsGridCellAttrProvider::GetAttr(row, col, kind);
}
