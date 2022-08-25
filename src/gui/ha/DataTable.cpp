#include "DataTable.h"

wxString DataTable::GetValue(int row, int col)
{
    return wxString::Format("%d", row * col);
}

wxString DataTable::GetRowLabelValue(int row)
{
    return wxString::Format("%d", row);
}

wxString DataTable::GetColLabelValue(int col)
{
    return wxString::Format("%d", col);
}

void DataTable::SetValue([[maybe_unused]] int row, [[maybe_unused]] int col, const wxString &value)
{
    m_value = value;
}
