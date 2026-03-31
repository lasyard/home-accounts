#include <wx/log.h>

#include "DataGridCellAttrProvider.h"

#include "DataDoc.h"
#include "DataTable.h"

#include "../HaGdi.h"

DataGridCellAttrProvider::DataGridCellAttrProvider(HaTable *table, const wxArrayString &accountNames)
    : HaGridCellAttrProvider(table)
{
    wxLog::AddTraceMask(TM);

    auto *renderer = new wxGridCellDateRenderer(_("%Y-%m-%d"));
    m_commentAttrRO->SetRenderer(renderer);

    m_deficitAttrRO = m_moneyAttrRO->Clone();
    m_deficitAttrRO->SetTextColour(HaGdi::DEFICIT_COLOR);

    m_accountChoiceAttr = m_defaultAttr->Clone();
    if (!accountNames.IsEmpty()) {
        m_accountChoiceAttr->SetEditor(new wxGridCellChoiceEditor(accountNames));
    }
}

DataGridCellAttrProvider::~DataGridCellAttrProvider()
{
    m_deficitAttrRO->DecRef();
    m_accountChoiceAttr->DecRef();
}

wxGridCellAttr *DataGridCellAttrProvider::GetItemCellAttr(int row, int col) const
{
    auto *table = static_cast<DataTable *>(m_table);
    if (table != nullptr) {
        switch (col) {
        case DataTable::ACCOUNT_COL:
            m_accountChoiceAttr->IncRef();
            return m_accountChoiceAttr;
        case DataTable::BALANCE_COL:
            if (table->GetDoc()->GetRecordBalance(table->GetRowRecord(row)) < 0) {
                m_deficitAttrRO->IncRef();
                return m_deficitAttrRO;
            }
            break;
        }
    }
    return HaGridCellAttrProvider::GetItemCellAttr(row, col);
}
