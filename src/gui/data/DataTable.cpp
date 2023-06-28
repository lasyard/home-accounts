#include <vector>

#include "DataTable.h"

#include "DataGridCellAttrProvider.h"

#include "../utils/DaoUtils.h"

#include "data/DataDao.h"
#include "data/ItemTraits.h"

const wxString DataTable::COL_LABELS[] = {
    t("Time"),
    t("Income"),
    t("Outlay"),
    t("Account"),
    t("Description"),
    t("Balance"),
    t("Valid"),
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
}

wxString DataTable::GetCellValue(int row, int col)
{
    std::string v;
    switch (GetRowType(row)) {
    case DataDao::IndexType::ITEM:
        switch (col) {
        case TIME_COL:
            v = m_dataDao->getTimeString(row);
            break;
        case INCOME_COL:
            v = m_dataDao->getIncomeString(row);
            break;
        case OUTLAY_COL:
            v = m_dataDao->getOutlayString(row);
            break;
        case ACCOUNT_COL:
            v = m_dataDao->getAccountString(row);
            break;
        case DESC_COL:
            v = m_dataDao->getDescString(row);
            break;
        case BALANCE_COL:
            v = m_dataDao->getBalanceString(row);
            break;
        case VALID_COL:
            v = m_dataDao->getValidString(row);
            break;
        default:
            break;
        }
        break;
    case DataDao::IndexType::INITIAL:
        if (col == BALANCE_COL) {
            v = m_dataDao->getBalanceString(row);
        }
        break;
    case DataDao::IndexType::PAGE:
        if (col == 0) {
            v = m_dataDao->getPageTitleString(row);
        }
        break;
    case DataDao::IndexType::OTHER:
        if (col == INCOME_COL) {
            v = m_dataDao->getTotalIncomeString();
        } else if (col == OUTLAY_COL) {
            v = m_dataDao->getTotalOutlayString();
        }
        break;
    }
    return wxString::FromUTF8(v);
}

void DataTable::SetCellValue(int row, int col, const wxString &value)
{
    auto const &v = value.utf8_string();
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
