#include <wx/log.h>

#include "DataGridCellAttrProvider.h"

#include "DataDoc.h"
#include "DataTable.h"

#include "../HaGdi.h"

DataGridCellAttrProvider::DataGridCellAttrProvider(HaTable *table) : HaGridCellAttrProvider(table)
{
    wxLog::AddTraceMask(TM);

    auto *renderer = new wxGridCellDateRenderer(_("%Y-%m-%d"));
    m_commentAttrRO->SetRenderer(renderer);

    m_deficitAttrRO = m_moneyAttrRO->Clone();
    m_deficitAttrRO->SetTextColour(HaGdi::DEFICIT_COLOR);
}

DataGridCellAttrProvider::~DataGridCellAttrProvider()
{
    m_deficitAttrRO->DecRef();
}

wxGridCellAttr *DataGridCellAttrProvider::GetItemCellAttr(int row, int col) const
{
    auto *table = static_cast<DataTable *>(m_table);
    if (table != nullptr && col == DataTable::BALANCE_COL &&
        table->GetDataDoc()->GetRecordBalance(table->GetRowRecord(row)) < 0) {
        m_deficitAttrRO->IncRef();
        return m_deficitAttrRO;
    }
    return HaGridCellAttrProvider::GetItemCellAttr(row, col);
}
