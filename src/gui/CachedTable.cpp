#include <vector>

#include "CachedTable.h"

CachedTable::CachedTable(int cols, const wxString colLabels[])
    : wxGridTableBase(), m_colLabels(cols, colLabels), m_cache(nullptr)
{
}

CachedTable::~CachedTable()
{
    if (m_cache != nullptr) {
        delete m_cache;
    }
}

void CachedTable::InitCache(int rows)
{
    m_cache = new wxVector<wxArrayString>(rows);
    for (auto i = 0; i < GetNumberRows(); ++i) {
        CacheRow(i);
    }
}

int CachedTable::GetNumberRows()
{
    return m_cache->size();
}

int CachedTable::GetNumberCols()
{
    return m_colLabels.size();
}

wxString CachedTable::GetValue(int row, int col)
{
    return (*m_cache)[row][col];
}

wxString CachedTable::GetColLabelValue(int col)
{
    return m_colLabels[col];
}

void CachedTable::SetValue(int row, int col, const wxString &value)
{
    SetCellValue(row, col, value.ToStdString());
    CacheCell(row, col);
    auto grid = GetView();
    if (grid != nullptr) {
        grid->AutoSizeColumn(col);
    }
}

bool CachedTable::InsertRows(size_t pos, size_t numRows)
{
    for (size_t i = 0; i < numRows; ++i) {
        if (!InsertRow(pos)) {
            break;
        }
        m_cache->insert(std::next(m_cache->begin(), pos), wxArrayString());
        CacheRow(pos);
    }
    auto grid = GetView();
    if (grid != nullptr) {
        wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_INSERTED, pos, numRows);
        grid->ProcessTableMessage(msg);
    }
    return true;
}

bool CachedTable::CanHaveAttributes()
{
    return true;
}

void CachedTable::CacheRow(int row)
{
    (*m_cache)[row].Empty();
    int cols = GetNumberCols();
    for (auto col = 0; col < cols; ++col) {
        (*m_cache)[row].Add(GetCellValue(row, col));
    }
}

void CachedTable::CacheCell(int row, int col)
{
    (*m_cache)[row][col] = GetCellValue(row, col);
}
