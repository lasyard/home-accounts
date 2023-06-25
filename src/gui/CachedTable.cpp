#include <vector>

#include <wx/log.h>

#include "CachedTable.h"

#include "data/CxxDefs.h"
#include "data/Dao.h"

CachedTable::CachedTable(size_t cols, const wxString colLabels[])
    : wxGridTableBase()
    , m_colLabels(cols, colLabels)
    , m_cache(nullptr)
{
}

CachedTable::~CachedTable()
{
    safe_delete(m_cache);
}

void CachedTable::CacheRow(int row)
{
    (*m_cache)[row].Empty();
    int cols = GetNumberCols();
    for (auto col = 0; col < cols; ++col) {
        (*m_cache)[row].Add(GetCellValue(row, col));
    }
}

void CachedTable::CacheCol(int col)
{
    for (auto i = 0; i < GetNumberRows(); ++i) {
        CacheCell(i, col);
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
    try {
        SetCellValue(row, col, value);
    } catch (std::runtime_error &e) {
        wxLogError(e.what());
    }
    CacheCell(row, col);
    RefreshAndAutoSizeGridColumn(col);
}

bool CachedTable::InsertRows(size_t pos, size_t numRows)
{
    size_t i;
    for (i = 0; i < numRows; ++i) {
        if (!InsertRow(pos)) {
            break;
        }
        m_cache->insert(std::next(m_cache->begin(), pos), wxArrayString());
        CacheRow(pos);
    }
    if (i > 0) {
        auto grid = GetView();
        if (grid != nullptr) {
            wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_INSERTED, pos, i);
            grid->ProcessTableMessage(msg);
        }
        return true;
    }
    return false;
}

bool CachedTable::AppendRows(size_t numRows)
{
    size_t i;
    for (i = 0; i < numRows; ++i) {
        if (!AppendRow()) {
            break;
        }
        m_cache->push_back(wxArrayString());
        CacheRow(m_cache->size() - 1);
    }
    if (i > 0) {
        auto grid = GetView();
        if (grid != nullptr && i > 0) {
            wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_APPENDED, i, 0);
            grid->ProcessTableMessage(msg);
        }
        return true;
    }
    return false;
}

bool CachedTable::DeleteRows(size_t pos, size_t numRows)
{
    size_t i;
    for (i = 0; i < numRows; ++i) {
        if (!DeleteRow(pos)) {
            break;
        }
    }
    m_cache->erase(std::next(m_cache->begin(), pos), std::next(m_cache->begin(), pos + i));
    if (i > 0) {
        auto grid = GetView();
        if (grid != nullptr && i > 0) {
            wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_DELETED, pos, i);
            grid->ProcessTableMessage(msg);
        }
        return true;
    }
    return false;
}

bool CachedTable::CanHaveAttributes()
{
    return true;
}

const std::string &CachedTable::GetDaoName() const
{
    return GetDao()->getName();
}

void CachedTable::CreateCache(int rows)
{
    safe_delete(m_cache);
    m_cache = new wxVector<wxArrayString>(rows);
    for (auto i = 0; i < GetNumberRows(); ++i) {
        CacheRow(i);
    }
}

void CachedTable::CacheCell(int row, int col)
{
    (*m_cache)[row][col] = GetCellValue(row, col);
}

void CachedTable::RefreshAndAutoSizeGridColumn(int col)
{
    auto grid = GetView();
    if (grid != nullptr) {
        grid->BeginBatch();
        grid->RefreshBlock(0, col, m_cache->size() - 1, col);
        grid->AutoSizeColumn(col);
        grid->EndBatch();
    }
}
