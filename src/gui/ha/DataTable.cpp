#include <vector>

#include "DataTable.h"
#include "csv/int.h"
#include "csv/money.h"

DataTable::DataTable(ColumnType types[], int cols) : wxGridTableBase(), m_cols(cols)
{
    m_types = new ColumnType[cols];
    for (int i = 0; i < cols; ++i) {
        m_types[i] = types[i];
    }
    AllocateVectors();
}

DataTable::~DataTable()
{
    ReleaseVectors();
    delete[] m_types;
}

wxString DataTable::GetValue(int row, int col)
{
    wxASSERT(0 <= col && col < m_cols && 0 <= row && row < m_rows);
    switch (m_types[col]) {
    case INT32:
        return wxString::Format("%d", static_cast<std::vector<int32_t> *>(m_values[col])->at(row));
    case INT64:
        return wxString::Format("%d", static_cast<std::vector<int64_t> *>(m_values[col])->at(row));
    case MONEY:
        return wxString::Format("%d", static_cast<std::vector<money_t> *>(m_values[col])->at(row));
    case STR:
        return static_cast<std::vector<std::string> *>(m_values[col])->at(row);
    default:
        return "";
    }
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
    wxASSERT(0 <= col && col < m_cols && 0 <= row && row < m_rows);
    switch (m_types[col]) {
    case INT32: {
        int32_t data;
        parse_int32(value.c_str(), &data, '\0');
        static_cast<std::vector<int32_t> *>(m_values[col])->at(row) = data;
    } break;
    case INT64: {
        int64_t data;
        parse_int64(value.c_str(), &data, '\0');
        static_cast<std::vector<int64_t> *>(m_values[col])->at(row) = data;
    } break;
    case MONEY: {
        money_t data;
        parse_money(value.c_str(), &data, '\0', 10);
        static_cast<std::vector<money_t> *>(m_values[col])->at(row) = data;
    } break;
    case STR: {
        static_cast<std::vector<std::string> *>(m_values[col])->at(row) = value.ToStdString();
    } break;
    default:
        break;
    }
}

void DataTable::AllocateVectors()
{
    m_values = new void *[m_cols];
    for (int i = 0; i < m_cols; ++i) {
        switch (m_types[i]) {
        case INT32:
            m_values[i] = new std::vector<int32_t>();
            break;
        case INT64:
            m_values[i] = new std::vector<int64_t>();
            break;
        case MONEY:
            m_values[i] = new std::vector<money_t>();
            break;
        case STR:
            m_values[i] = new std::vector<std::string>();
            break;
        default:
            m_values[i] = nullptr;
            break;
        }
    }
}

void DataTable::ReleaseVectors()
{
    for (int i = 0; i < m_cols; ++i) {
        switch (m_types[i]) {
        case INT32:
            delete static_cast<std::vector<int32_t> *>(m_values[i]);
            break;
        case INT64:
            delete static_cast<std::vector<int64_t> *>(m_values[i]);
            break;
        case MONEY:
            delete static_cast<std::vector<money_t> *>(m_values[i]);
            break;
        case STR:
            delete static_cast<std::vector<std::string> *>(m_values[i]);
            break;
        default:
            break;
        }
    }
    delete[] m_values;
}
