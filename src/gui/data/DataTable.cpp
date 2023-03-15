#include <vector>

#include "../CellAttrs.h"
#include "DataTable.h"
#include "data/DataDao.h"

const wxString DataTable::COL_LABELS[] = {
    _("Time"),
    _("Income"),
    _("Outlay"),
    _("Description"),
};

DataTable::DataTable(DataDao *dataDao) : CachedTable(COL_NUM, COL_LABELS), m_dataDao(dataDao)
{
    m_pageTitleAttr = CellAttrs::ins().GetReadOnly()->Clone();
    m_pageTitleAttr->SetSize(1, COL_NUM);
    m_pageTitleAttr->SetFont(CellAttrs::ins().GetMonoFont());
    m_pageTitleAttr->SetBackgroundColour(*wxLIGHT_GREY);
    m_pageTitleAttr->SetAlignment(wxALIGN_CENTER_VERTICAL, wxALIGN_LEFT);
    // Do not call `this->GetNumberRows()`, cache is not inited.
    InitCache(dataDao->getNumberRows());
}

DataTable::~DataTable()
{
    m_pageTitleAttr->DecRef();
}

wxString DataTable::GetRowLabelValue(int row)
{
    return m_dataDao->getRowLabel(row);
}

wxGridCellAttr *DataTable::GetAttr(int row, int col, [[maybe_unused]] wxGridCellAttr::wxAttrKind kind)
{
    auto rowType = m_dataDao->getRowType(row);
    if (rowType == DataDao::IndexType::ITEM) {
        switch (col) {
        case TIME_COL:
            return CellAttrs::ins().GetTime();
        case INCOME_COL:
        case OUTLAY_COL:
            return CellAttrs::ins().GetMoney();
        default:
            break;
        }
    } else if (rowType == DataDao::IndexType::PAGE) {
        // Do not return colSpan > 1 for col > 0, or there will be index out of bond problem.
        return col == 0 ? GetPageTitleAttr() : CellAttrs::ins().GetOverlapped();
    }
    return CellAttrs::ins().GetDefault();
}

wxString DataTable::GetCellValue(int row, int col)
{
    auto rowType = m_dataDao->getRowType(row);
    if (rowType == DataDao::IndexType::ITEM) {
        switch (col) {
        case TIME_COL:
            return m_dataDao->getTimeString(row);
        case INCOME_COL:
            return m_dataDao->getIncomeString(row);
        case OUTLAY_COL:
            return m_dataDao->getOutlayString(row);
        case DESC_COL:
            return m_dataDao->getDescString(row);
        default:
            break;
        }
    } else if (rowType == DataDao::IndexType::PAGE && col == 0) {
        return m_dataDao->getPageTitleString(row);
    }
    return "";
}

void DataTable::SetCellValue(int row, int col, const std::string &value)
{
    switch (col) {
    case INCOME_COL:
        m_dataDao->setMoney(row, value, true);
        // Update outlay, too
        CacheCell(row, OUTLAY_COL);
        break;
    case OUTLAY_COL:
        m_dataDao->setMoney(row, value, false);
        // Update income, too
        CacheCell(row, INCOME_COL);
        break;
    case DESC_COL:
        m_dataDao->setDesc(row, value);
        break;
    default:
        break;
    }
}

bool DataTable::InsertRow(size_t pos)
{
    return m_dataDao->insertItemAfter(pos - 1);
}

bool DataTable::AppendRow()
{
    // Never append.
    return false;
}

wxGridCellAttr *DataTable::GetPageTitleAttr()
{
    m_pageTitleAttr->IncRef();
    return m_pageTitleAttr;
}
