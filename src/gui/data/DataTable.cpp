#include <vector>

#include "DataTable.h"

#include "DataGridCellAttrProvider.h"

#include "../utils/DaoUtils.h"

#include "data/DataDao.h"
#include "data/ItemTraits.h"

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
    CreateCache(m_dataDao->getNumberRows() + 1);
    SetAttrProvider(new DataGridCellAttrProvider(this));
}

DataTable::~DataTable()
{
}

wxString DataTable::GetRowLabelValue(int row)
{
    if (row < m_dataDao->getNumberRows()) {
        return m_dataDao->getRowLabel(row);
    } else if (row == m_dataDao->getNumberRows()) {
        return _("Total");
    }
    return "";
}

DataDao *DataTable::GetDao()
{
    return m_dataDao;
}

const DataDao *DataTable::GetDao() const
{
    return m_dataDao;
}

ColumnType DataTable::GetColumnType(int col) const
{
    return CsvRowTraits<struct item>::types[col];
}

void DataTable::UpdateChoicesFromJoints()
{
    wxArrayString choices;
    Utils::GetStrings(choices, m_dataDao->getAccountJoint());
    static_cast<DataGridCellAttrProvider *>(GetAttrProvider())->SetAccountChoices(choices);
    Utils::GetStrings(choices, m_dataDao->getChannelJoint());
    static_cast<DataGridCellAttrProvider *>(GetAttrProvider())->SetChannelChoices(choices);
}

wxString DataTable::GetCellValue(int row, int col)
{
    switch (GetRowType(row)) {
    case DataDao::IndexType::ITEM:
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
        break;
    case DataDao::IndexType::INITIAL:
        if (col == BALANCE_COL) {
            return m_dataDao->getBalanceString(row);
        }
        break;
    case DataDao::IndexType::PAGE:
        if (col == 0) {
            return m_dataDao->getPageTitleString(row);
        }
        break;
    case DataDao::IndexType::OTHER:
        if (col == INCOME_COL) {
            return m_dataDao->getTotalIncomeString();
        } else if (col == OUTLAY_COL) {
            return m_dataDao->getTotalOutlayString();
        }
        break;
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
        CacheBalances(row);
        CacheTotal();
        break;
    case OUTLAY_COL:
        m_dataDao->setMoney(row, v, false);
        // Update income, too
        CacheCell(row, INCOME_COL);
        // Balances after this row are changed.
        CacheBalances(row);
        CacheTotal();
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
        CacheBalances(row);
        CacheTotal();
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

void DataTable::CacheBalances(int row)
{
    for (int i = row; i < GetNumberRows(); ++i) {
        CacheCell(i, BALANCE_COL);
    }
    RefreshAndAutoSizeGridColumn(BALANCE_COL);
}

void DataTable::CacheTotal()
{
    CacheCell(m_dataDao->getNumberRows(), INCOME_COL);
    CacheCell(m_dataDao->getNumberRows(), OUTLAY_COL);
}
