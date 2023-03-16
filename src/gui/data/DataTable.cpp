#include <vector>

#include "DataTable.h"
#include "data/DataDao.h"

const wxString DataTable::COL_LABELS[] = {
    _("Time"),
    _("Income"),
    _("Outlay"),
    _("Account"),
    _("Channel"),
    _("Description"),
    _("Valid"),
};

DataTable::DataTable(DataDao *dataDao) : CachedTable(COL_NUM, COL_LABELS), m_dataDao(dataDao)
{
    m_pageTitleAttr = CellAttrs::ins().CloneReadOnly();
    m_pageTitleAttr->SetSize(1, COL_NUM);
    m_pageTitleAttr->SetFont(CellAttrs::ins().GetMonoFont());
    m_pageTitleAttr->SetBackgroundColour(*wxLIGHT_GREY);
    m_pageTitleAttr->SetAlignment(wxALIGN_CENTER_VERTICAL, wxALIGN_LEFT);
    m_accountAttr = CellAttrs::ins().CloneReadOnly();
    m_channelAttr = CellAttrs::ins().CloneReadOnly();
    // Do not call `this->GetNumberRows()`, cache is not inited.
    InitCache(dataDao->getNumberRows());
}

DataTable::~DataTable()
{
    m_pageTitleAttr->DecRef();
    m_accountAttr->DecRef();
    m_channelAttr->DecRef();
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
        case ACCOUNT_COL:
            return GetAccountAttr();
        case CHANNEL_COL:
            return GetChannelAttr();
        case VALID_COL:
            return CellAttrs::ins().GetBool();
        default:
            break;
        }
    } else if (rowType == DataDao::IndexType::PAGE) {
        // Do not return colSpan > 1 for col > 0, or there will be index out of bond problem.
        return col == 0 ? GetPageTitleAttr() : CellAttrs::ins().GetOverlapped();
    }
    return CellAttrs::ins().GetDefault();
}

void DataTable::SetAccountChoices(wxArrayString &choices)
{
    SetChoicesOf(m_accountAttr, choices);
}

void DataTable::SetChannelChoices(wxArrayString &choices)
{
    SetChoicesOf(m_channelAttr, choices);
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
        case ACCOUNT_COL:
            return m_dataDao->getAccountString(row);
        case CHANNEL_COL:
            return m_dataDao->getChannelString(row);
        case DESC_COL:
            return m_dataDao->getDescString(row);
        case VALID_COL:
            return m_dataDao->getValidString(row);
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
    case ACCOUNT_COL:
        m_dataDao->setAccount(row, value);
        break;
    case CHANNEL_COL:
        m_dataDao->setChannel(row, value);
        break;
    case DESC_COL:
        m_dataDao->setDesc(row, value);
        break;
    case VALID_COL:
        m_dataDao->setValid(row, value);
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
