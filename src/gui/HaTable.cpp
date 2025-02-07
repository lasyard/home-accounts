#include <wx/log.h>

#include "HaTable.h"

#include "HaGridCellAttrProvider.h"

HaTable::HaTable(CsvDoc *doc) : wxGridTableBase(), m_doc(doc), m_cache(nullptr)
{
    m_doc->GetColLabels(m_colLabels);
    SetAttrProvider(new HaGridCellAttrProvider(this));
}

HaTable::~HaTable()
{
    if (m_cache != nullptr) {
        delete m_cache;
    }
}

int HaTable::GetNumberRows()
{
    return m_cache != nullptr ? m_cache->size() : 0;
}

int HaTable::GetNumberCols()
{
    return m_colLabels.size();
}

wxString HaTable::GetValue(int row, int col)
{
    return (*m_cache)[row][col];
}

wxString HaTable::GetColLabelValue(int col)
{
    return m_colLabels[col];
}

wxString HaTable::GetRowLabelValue([[maybe_unused]] int row)
{
    return wxEmptyString;
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

bool HaTable::CanHaveAttributes()
{
    return true;
}

void HaTable::CacheCell(int row, int col)
{
    (*m_cache)[row][col] = GetCellValue(row, col);
}

void HaTable::CacheRow(int row)
{
    (*m_cache)[row].Empty();
    int cols = GetNumberCols();
    for (auto col = 0; col < cols; ++col) {
        (*m_cache)[row].Add(GetCellValue(row, col));
    }
}

void HaTable::CacheCol(int col)
{
    for (auto i = 0; i < GetNumberRows(); ++i) {
        CacheCell(i, col);
    }
}

void HaTable::RefreshAndAutoSizeGridColumn(int col)
{
    auto grid = GetView();
    if (grid != nullptr) {
        grid->BeginBatch();
        grid->RefreshBlock(0, col, m_cache->size() - 1, col);
        grid->AutoSizeColumn(col);
        grid->EndBatch();
    }
}
