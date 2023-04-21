#include "ConfigsGridCellAttrProvider.h"
#include "../CsvTable.h"

ConfigsGridCellAttrProvider::ConfigsGridCellAttrProvider(const CachedTable *table)
    : HaGridCellAttrProvider(), m_table(table)
{
}

ConfigsGridCellAttrProvider::~ConfigsGridCellAttrProvider()
{
}

wxGridCellAttr *
ConfigsGridCellAttrProvider::GetAttr([[maybe_unused]] int row, int col, wxGridCellAttr::wxAttrKind kind) const
{
    // Seems `kind` is always `Any`.
    if (kind == wxGridCellAttr::wxAttrKind::Any || kind == wxGridCellAttr::wxAttrKind::Cell) {
        auto type = m_table->GetColumnType(col);
        switch (type) {
        case INT32:
        case INT64:
        case DATE:
        case TIME:
            m_monoAttr->IncRef();
            return m_monoAttr;
        case MONEY:
            m_moneyAttr->IncRef();
            return m_moneyAttr;
        default:
            break;
        }
    }
    m_defaultAttr->IncRef();
    return m_defaultAttr;
}
