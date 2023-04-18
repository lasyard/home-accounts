#include <vector>

#include "DataGridCellAttrProvider.h"
#include "DataTable.h"
#include "data/DataDao.h"

const wxString DataTable::COL_LABELS[] = {
    _("Time"),
    _("Income"),
    _("Outlay"),
    _("Account"),
    _("Channel"),
    _("Description"),
    _("Balance"),
    _("Valid"),
};

DataTable::DataTable(DataDao *dataDao) : CachedTable(COL_NUM, COL_LABELS), m_dataDao(dataDao)
{
    // Do not call `this->GetNumberRows()`, cache is not inited.
    InitCache(dataDao->getNumberRows());
    SetAttrProvider(new DataGridCellAttrProvider(this));
}

DataTable::~DataTable()
{
}

wxString DataTable::GetRowLabelValue(int row)
{
    return m_dataDao->getRowLabel(row);
}

void DataTable::SetAccountChoices(wxArrayString &choices)
{
    static_cast<DataGridCellAttrProvider *>(GetAttrProvider())->SetAccountChoices(choices);
}

void DataTable::SetChannelChoices(wxArrayString &choices)
{
    static_cast<DataGridCellAttrProvider *>(GetAttrProvider())->SetChannelChoices(choices);
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
        case BALANCE_COL:
            return m_dataDao->getBalanceString(row);
        case VALID_COL:
            return m_dataDao->getValidString(row);
        default:
            break;
        }
    } else if (rowType == DataDao::IndexType::INITIAL && col == BALANCE_COL) {
        return m_dataDao->getBalanceString(row);
    } else if (rowType == DataDao::IndexType::PAGE && col == 0) {
        return m_dataDao->getPageTitleString(row);
    }
    return wxEmptyString;
}

void DataTable::SetCellValue(int row, int col, const wxString &value)
{
    auto const &v = value.ToStdString();
    switch (col) {
    case INCOME_COL:
        m_dataDao->setMoney(row, v, true);
        // Update outlay, too
        CacheCell(row, OUTLAY_COL);
        // Balances after this row are changed.
        ReCacheBalances(row);
        break;
    case OUTLAY_COL:
        m_dataDao->setMoney(row, v, false);
        // Update income, too
        CacheCell(row, INCOME_COL);
        // Balances after this row are changed.
        ReCacheBalances(row);
        break;
    case ACCOUNT_COL:
        m_dataDao->setAccount(row, v);
        break;
    case CHANNEL_COL:
        m_dataDao->setChannel(row, v);
        break;
    case DESC_COL:
        m_dataDao->setDesc(row, v);
        break;
    case VALID_COL:
        m_dataDao->setValid(row, v);
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

void DataTable::ReCacheBalances(int row)
{
    for (int i = row; i < GetNumberRows(); ++i) {
        CacheCell(i, BALANCE_COL);
    }
}
