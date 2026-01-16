#include <wx/log.h>

#include <stdexcept>

#include "HaTable.h"

#include "HaGridCellAttrProvider.h"

HaTable::~HaTable()
{
    if (m_cache != nullptr) {
        delete m_cache;
    }
    if (m_doc != nullptr) {
        delete m_doc;
    }
    delete[] m_colImpls;
}

void HaTable::Init()
{
    int rows = (m_doc != nullptr ? m_doc->GetRowCount() : 0);
    m_cache = new wxVector<wxArrayString>(rows);
    for (auto i = 0; i < rows; ++i) {
        CacheRow(i);
    }
    SetAttrProvider(new HaGridCellAttrProvider(this));
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

wxString HaTable::GetRowLabelValue(int row)
{
    if (row < GetNumberRows()) {
        return wxString::Format("%d", row);
    }
    return wxEmptyString;
}

bool HaTable::CanHaveAttributes()
{
    return true;
}

enum column_type HaTable::GetColType(int col) const
{
    return col < m_cols ? m_colImpls[col].type : CT_IGNORE;
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

wxString HaTable::GetCommentString(int row)
{
    return m_doc->GetValueString(row, 0);
}

bool HaTable::InsertRow(size_t pos)
{
    auto *record = m_doc->InsertRecord(pos);
    return record != nullptr;
}

bool HaTable::AppendRow()
{
    auto *record = m_doc->AddRecord();
    return record != nullptr;
}

bool HaTable::DeleteRow(size_t pos)
{
    return m_doc->DeleteRecord(pos);
}
