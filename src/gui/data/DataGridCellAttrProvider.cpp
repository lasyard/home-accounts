#include <wx/log.h>

#include "DataGridCellAttrProvider.h"

#include "DataTable.h"

#include "../HaGdi.h"

#include "data/data.h"

IMPLEMENT_TM(DataGridCellAttrProvider)

DataGridCellAttrProvider::DataGridCellAttrProvider(const HaTable *table) : HaGridCellAttrProvider(table)
{
    wxLog::AddTraceMask(TM);

    m_roMoneyAttr = m_moneyAttr->Clone();
    m_roMoneyAttr->SetReadOnly();

    m_roDeficitAttr = m_roMoneyAttr->Clone();
    m_roDeficitAttr->SetTextColour(HaGdi::DEFICIT_COLOR);
}

DataGridCellAttrProvider::~DataGridCellAttrProvider()
{
    m_roMoneyAttr->DecRef();
    m_roDeficitAttr->DecRef();
}

wxGridCellAttr *DataGridCellAttrProvider::GetItemCellAttr(int row, int col) const
{
    if (col == DataTable::BALANCE_COL) {
        auto *table = static_cast<const DataTable *>(m_table);
        struct data *data = table->GetData(row);
        if (data->balance < 0) {
            m_roDeficitAttr->IncRef();
            return m_roDeficitAttr;
        }
        m_roMoneyAttr->IncRef();
        return m_roMoneyAttr;
    }
    return HaGridCellAttrProvider::GetItemCellAttr(row, col);
}
