#include <vector>

#include "DataTable.h"
#include "data/DataFile.h"
#include "data/item.h"
#include "data/page.h"

DataTable::DataTable(DataFile *dataFile) : m_dataFile(dataFile)
{
}

DataTable::~DataTable()
{
}

wxString DataTable::GetValue(int row, int col)
{
    return m_dataFile->getString(row, col);
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
}

wxGridCellAttr *DataTable::GetAttr([[maybe_unused]] int row, int col, [[maybe_unused]] wxGridCellAttr::wxAttrKind kind)
{
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
