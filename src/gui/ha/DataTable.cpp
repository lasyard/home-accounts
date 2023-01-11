#include <vector>

#include "DataTable.h"
#include "data/DataFile.h"
#include "data/item.h"
#include "data/page.h"

const wxString DataTable::COL_LABELS[]{
    "Time",
    "Income",
    "Outlay",
    "Description",
};

DataTable::DataTable(DataFile *dataFile)
    : wxGridTableBase(), m_dataFile(dataFile), m_attrs(COL_NUM), m_cache(GetNumberRows(), wxArrayString())
{
    for (auto i = 0; i < GetNumberRows(); ++i) {
        CacheRow(i);
    }
}

DataTable::~DataTable()
{
}

wxString DataTable::GetValue(int row, int col)
{
    return m_cache[row][col];
}

wxString DataTable::GetRowLabelValue(int row)
{
    return m_dataFile->getRowLabel(row);
}

wxString DataTable::GetColLabelValue(int col)
{
    return _(COL_LABELS[col]);
}

void DataTable::SetValue(int row, int col, const wxString &value)
{
    SetCellValue(row, col, value.ToStdString());
    m_cache[row][col] = GetCellValue(row, col);
    auto grid = GetView();
    if (grid != nullptr) {
        grid->AutoSizeColumn(col);
    }
}

bool DataTable::InsertRows(size_t pos, size_t numRows)
{
    wxASSERT(pos > 0);
    for (size_t i = 0; i < numRows; ++i) {
        if (!m_dataFile->insertItemAfter(pos - 1)) {
            break;
        }
        m_cache.insert(std::next(m_cache.begin(), pos), wxArrayString());
        CacheRow(pos);
    }
    auto grid = GetView();
    if (grid != nullptr) {
        wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_INSERTED, pos, numRows);
        grid->ProcessTableMessage(msg);
    }
    return true;
}

wxGridCellAttr *DataTable::GetAttr(int row, int col, [[maybe_unused]] wxGridCellAttr::wxAttrKind kind)
{
    auto rowType = m_dataFile->getRowType(row);
    if (rowType == DataFile::IndexType::ITEM) {
        switch (col) {
        case TIME_COL:
            return m_attrs.GetTime();
        case INCOME_COL:
        case OUTLAY_COL:
            return m_attrs.GetMoney();
        default:
            break;
        }
    } else if (rowType == DataFile::IndexType::PAGE) {
        // Do not return colSpan > 1 for col > 0, or there will be index out of bond problem.
        return col == 0 ? m_attrs.GetPage() : m_attrs.GetOverlapped();
    }
    return m_attrs.GetDefault();
}

void DataTable::CacheRow(int row)
{
    m_cache[row].Empty();
    auto rowType = m_dataFile->getRowType(row);
    if (rowType == DataFile::IndexType::ITEM) {
        for (auto col = 0; col < COL_NUM; ++col) {
            m_cache[row].Add(GetCellValue(row, col));
        }
    } else if (rowType == DataFile::IndexType::PAGE) {
        m_cache[row].Add(m_dataFile->getPageTitleString(row));
        for (auto col = 1; col < COL_NUM; ++col) {
            m_cache[row].Add("");
        }
    }
}

wxString DataTable::GetCellValue(int row, int col)
{
    switch (col) {
    case TIME_COL:
        return m_dataFile->getTimeString(row);
    case INCOME_COL:
        return m_dataFile->getIncomeString(row);
    case OUTLAY_COL:
        return m_dataFile->getOutlayString(row);
    case DESC_COL:
        return m_dataFile->getDescString(row);
    default:
        break;
    }
    return "";
}

void DataTable::SetCellValue(int row, int col, const std::string &value)
{
    switch (col) {
    case INCOME_COL:
        m_dataFile->setMoney(row, value, true);
        // Update outlay, too
        m_cache[row][OUTLAY_COL] = m_dataFile->getOutlayString(row);
        break;
    case OUTLAY_COL:
        m_dataFile->setMoney(row, value, false);
        // Update income, too
        m_cache[row][INCOME_COL] = m_dataFile->getIncomeString(row);
        break;
    case DESC_COL:
        m_dataFile->setDesc(row, value);
        break;
    default:
        break;
    }
}
