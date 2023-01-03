#include <vector>

#include "DataTable.h"
#include "data/DataFile.h"
#include "data/item.h"
#include "data/page.h"

DataTable::DataTable(DataFile *dataFile)
    : wxGridTableBase(),
      m_dataFile(dataFile),
      m_attrs(dataFile->getNumberCols()),
      m_cache(dataFile->getNumberRows(), wxArrayString(dataFile->getNumberCols(), _(""))),
      m_flag(dataFile->getNumberRows(), wxVector(dataFile->getNumberCols(), false))
{
}

DataTable::~DataTable()
{
}

wxString DataTable::GetValue(int row, int col)
{
    if (!m_flag[row][col]) {
        m_cache[row][col] = m_dataFile->getString(row, col);
        m_flag[row][col] = true;
    }
    return m_cache[row][col];
}

wxString DataTable::GetRowLabelValue(int row)
{
    return wxString::Format("%d", row);
}

wxString DataTable::GetColLabelValue(int col)
{
    return wxString::Format("%d", col);
}

void DataTable::SetValue(int row, int col, const wxString &value)
{
    m_dataFile->setString(row, col, value.ToStdString());
    m_flag[row][col] = false;
}

wxGridCellAttr *DataTable::GetAttr([[maybe_unused]] int row, int col, [[maybe_unused]] wxGridCellAttr::wxAttrKind kind)
{
    // Do not return colSpan > 1 for col > 0, or there will be index out of bond problem.
    if (m_dataFile->isPage(row)) {
        return col == 0 ? m_attrs.GetPage() : m_attrs.GetOverlapped();
    }
    switch (m_dataFile->getTypes()[col]) {
    case MONEY:
        return m_attrs.GetMoney();
    case TIME:
        return m_attrs.GetTime();
    default:
        break;
    }
    return m_attrs.GetDefault();
}
