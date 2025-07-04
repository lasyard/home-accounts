#include <wx/log.h>

#include "AccountGridCellAttrProvider.h"

#include "AccountTable.h"

IMPLEMENT_TM(AccountGridCellAttrProvider)

AccountGridCellAttrProvider::AccountGridCellAttrProvider(const HaTable *table) : HaGridCellAttrProvider(table)
{
    wxLog::AddTraceMask(TM);
}

AccountGridCellAttrProvider::~AccountGridCellAttrProvider()
{
}

wxGridCellAttr *AccountGridCellAttrProvider::GetItemCellAttr(int row, int col) const
{
    if (col == AccountTable::ID_COL) {
        integerAttrRO->IncRef();
        return integerAttrRO;
    }
    return HaGridCellAttrProvider::GetItemCellAttr(row, col);
}
