#include <wx/log.h>

#include "DataGridCellAttrProvider.h"

#include "DataTable.h"

#include "data/data.h"

IMPLEMENT_TM(DataGridCellAttrProvider)

DataGridCellAttrProvider::DataGridCellAttrProvider(const HaTable *table) : HaGridCellAttrProvider(table)
{
    wxLog::AddTraceMask(TM);
}

DataGridCellAttrProvider::~DataGridCellAttrProvider()
{
}

wxGridCellAttr *DataGridCellAttrProvider::GetItemCellAttr(int row, int col) const
{
    if (col == DataTable::BALANCE_COL) {
        auto *table = static_cast<const DataTable *>(m_table);
        struct data *data = table->GetData(row);
        if (data->balance < 0) {
            deficitAttrRO->IncRef();
            return deficitAttrRO;
        }
        moneyAttrRO->IncRef();
        return moneyAttrRO;
    }
    return HaGridCellAttrProvider::GetItemCellAttr(row, col);
}
