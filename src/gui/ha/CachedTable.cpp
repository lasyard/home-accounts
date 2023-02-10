#include <vector>

#include "CachedTable.h"
#include "data/DataDao.h"
#include "data/item.h"
#include "data/page.h"

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
    wxASSERT(pos > 0);
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
