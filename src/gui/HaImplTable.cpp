#include "HaImplTable.h"

void HaImplTable::MapColImpl(struct ColImpl &colImpl, int col)
{
    colImpl.type = m_doc->GetItemValueType(col);
    colImpl.get = [this, col](int row) -> wxString { return HaTable::GetItemCellValue(row, col); };
    colImpl.set = [this, col](int row, const wxString &value) -> void {
        return HaTable::SetItemCellValue(row, col, value);
    };
}

const wxString HaImplTable::GetItemCellValue(int row, int col)
{
    if (col < m_columns) {
        if (m_colImpls[col].get != nullptr) {
            return m_colImpls[col].get(row);
        }
        return _("not implemented");
    }
    return wxEmptyString;
}

void HaImplTable::SetItemCellValue(int row, int col, const wxString &value)
{
    if (col < m_columns && m_colImpls[col].set != nullptr) {
        m_colImpls[col].set(row, value);
    }
}
