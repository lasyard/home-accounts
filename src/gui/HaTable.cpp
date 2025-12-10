#include <wx/log.h>

#include <stdexcept>

#include "HaTable.h"

#include "HaGridCellAttrProvider.h"

void HaTable::Init()
{
    int rows = m_doc != nullptr ? m_doc->GetRecordCount() : 0;
    for (auto i = 0; i < rows; ++i) {
        CacheRow(i);
    }
    SetAttrProvider(new HaGridCellAttrProvider(this));
}

void HaTable::SetValue(int row, int col, const wxString &value)
{
    try {
        SetCellValue(row, col, value);
    } catch (std::runtime_error &e) {
        wxLogError(e.what());
    }
    CacheCell(row, col);
    RefreshAndAutoSizeGridColumn(col);
}

bool HaTable::InsertRows(size_t pos, size_t numRows)
{
    size_t i;
    for (i = 0; i < numRows; ++i) {
        if (!InsertRow(pos - 1)) {
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

bool HaTable::AppendRows(size_t numRows)
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
        if (grid != nullptr) {
            wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_APPENDED, i, 0);
            grid->ProcessTableMessage(msg);
        }
        return true;
    }
    return false;
}

bool HaTable::DeleteRows(size_t pos, size_t numRows)
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
        if (grid != nullptr) {
            wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_DELETED, pos, i);
            grid->ProcessTableMessage(msg);
        }
        return true;
    }
    return false;
}

bool HaTable::InsertRow(size_t pos)
{
    auto *record = m_doc->InsertRecord(pos);
    return record != nullptr;
}

bool HaTable::AppendRow()
{
    // never append
    return false;
}

bool HaTable::DeleteRow(size_t pos)
{
    return m_doc->DeleteRecord(pos);
}
