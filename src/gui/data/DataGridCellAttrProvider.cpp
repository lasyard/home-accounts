#include "DataGridCellAttrProvider.h"
#include "DataTable.h"
#include "data/DataDao.h"

DataGridCellAttrProvider::DataGridCellAttrProvider(const DataTable *table) : HaGridCellAttrProvider(), m_table(table)
{
    m_pageTitleAttr = m_monoAttr->Clone();
    m_pageTitleAttr->SetSize(1, DataTable::COL_NUM);
    m_pageTitleAttr->SetBackgroundColour(*wxLIGHT_GREY);
    m_pageTitleAttr->SetAlignment(wxALIGN_CENTER_VERTICAL, wxALIGN_LEFT);

    m_timeAttr = m_readOnlyAttr->Clone();
    m_timeAttr->SetFont(m_monoFont.Smaller());
    m_timeAttr->SetTextColour(*wxBLUE);

    m_accountAttr = m_readOnlyAttr->Clone();

    m_channelAttr = m_readOnlyAttr->Clone();
}

DataGridCellAttrProvider::~DataGridCellAttrProvider()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    m_pageTitleAttr->DecRef();
    m_timeAttr->DecRef();
    m_accountAttr->DecRef();
    m_channelAttr->DecRef();
}

wxGridCellAttr *DataGridCellAttrProvider::GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) const
{
    // Seems `kind` is always `Any`.
    if (kind == wxGridCellAttr::wxAttrKind::Any || kind == wxGridCellAttr::wxAttrKind::Cell) {
        auto rowType = m_table->GetRowType(row);
        if (rowType == DataDao::IndexType::ITEM) {
            switch (col) {
            case DataTable::TIME_COL:
                m_timeAttr->IncRef();
                return m_timeAttr;
            case DataTable::INCOME_COL:
            case DataTable::OUTLAY_COL:
                m_moneyAttr->IncRef();
                // wxLogTrace(TM, "RefCount of moneyAttr is %d", m_moneyAttr->GetRefCount());
                return m_moneyAttr;
            case DataTable::ACCOUNT_COL:
                m_accountAttr->IncRef();
                // wxLogTrace(TM, "RefCount of accountAttr is %d", m_accountAttr->GetRefCount());
                return m_accountAttr;
            case DataTable::CHANNEL_COL:
                m_channelAttr->IncRef();
                // wxLogTrace(TM, "RefCount of channelAttr is %d", m_channelAttr->GetRefCount());
                return m_channelAttr;
            case DataTable::VALID_COL:
                m_boolAttr->IncRef();
                return m_boolAttr;
            default:
                break;
            }
        } else if (rowType == DataDao::IndexType::PAGE) {
            // Do not return colSpan > 1 for col > 0, or there will be index out of bond problem.
            if (col == 0) {
                m_pageTitleAttr->IncRef();
                return m_pageTitleAttr;
            } else {
                m_readOnlyAttr->IncRef();
                return m_readOnlyAttr;
            }
        }
    }
    m_defaultAttr->IncRef();
    return m_defaultAttr;
}
