#include <wx/log.h>

#include <stdexcept>

#include "HaTable.h"

#include "data/StdStreamAccessor.h"

HaTable::~HaTable()
{
    if (m_cache != nullptr) {
        delete m_cache;
    }
    if (m_doc != nullptr) {
        delete m_doc;
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
    return _(m_colLabels[col]);
}

wxString HaTable::GetRowLabelValue(int row)
{
    if ((size_t)row < m_index.size()) {
        const HaTableIndex &item = m_index[row];
        if (item.GetType() == HaTableIndex::ITEM) {
            return wxString::Format("%d", item.GetSeq());
        }
    }
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

void HaTable::SaveTo(std::ostream &os)
{
    m_doc->Write(stream_writer, &os);
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

void HaTable::Init()
{
    m_index.clear();
    if (m_doc != nullptr) {
        m_doc->ForEachSegment([this](struct segment *segment) -> bool {
            m_index.push_back(HaTableIndex(segment));
            int seq = 1;
            m_doc->ForEachItem(segment, [this, segment, &seq](void *item) -> bool {
                m_index.push_back(HaTableIndex(segment, item, seq++));
                return true;
            });
            return true;
        });
    }
    int rows = m_index.size();
    m_cache = new wxVector<wxArrayString>(rows);
    for (auto i = 0; i < rows; ++i) {
        CacheRow(i);
    }
    SetAttrProvider(new HaGridCellAttrProvider(this));
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

const wxString HaTable::GetCellValue(int row, int col)
{
    switch (GetRowType(row)) {
    case HaTableIndex::ITEM:
        if ((size_t)col < m_colLabels.size()) {
            return GetItemCellValue(row, col);
        }
        break;
    case HaTableIndex::SEGMENT:
        if (col == 0) {
            return GetSegmentCellValue(row);
        }
        break;
    case HaTableIndex::INVALID:
        break;
    }
    return wxEmptyString;
}

const wxString HaTable::GetItemCellValue(int row, int col)
{
    return m_doc->GetItemValueString(m_index[row].GetItem(), col);
}

const wxString HaTable::GetItemCellMoneyValueBySign(int row, int col, bool negative)
{
    return m_doc->GetItemMoneyStringBySign(m_index[row].GetItem(), col, negative);
}

const wxString HaTable::GetSegmentCellValue(int row)
{
    return m_doc->GetSegmentValueString(m_index[row].GetSegment());
}

void HaTable::SetCellValue(int row, int col, const wxString &value)
{
    switch (GetRowType(row)) {
    case HaTableIndex::ITEM:
        if ((size_t)col < m_colLabels.size()) {
            SetItemCellValue(row, col, value);
        }
        break;
    case HaTableIndex::SEGMENT:
        if (col == 0) {
            SetSegmentCellValue(row, value);
        }
        break;
    case HaTableIndex::INVALID:
        break;
    }
}

void HaTable::SetItemCellValue(int row, int col, const wxString &value)
{
    m_doc->SetItemValueString(m_index[row].GetItem(), col, value);
}

void HaTable::SetSegmentCellValue(int row, const wxString &value)
{
    m_doc->SetSegmentValueString(m_index[row].GetSegment(), value);
}

bool HaTable::InsertRow(size_t pos)
{
    auto &index = m_index[pos];
    void *item = nullptr;
    if (index.GetType() == HaTableIndex::ITEM) {
        item = m_doc->InsertItem(index.GetItem());
    } else if (index.GetType() == HaTableIndex::SEGMENT) {
        item = m_doc->InsertItemHead(index.GetSegment());
    }
    if (item != nullptr) {
        int seq = index.GetSeq() + 1;
        m_index.insert(std::next(m_index.begin(), pos + 1), HaTableIndex(index.GetSegment(), item, seq));
        for (auto p = pos + 2; p < m_index.size() && m_index[p].GetType() != HaTableIndex::SEGMENT; ++p) {
            ++m_index[p].GetSeq();
        }
        return true;
    }
    return false;
}

bool HaTable::AppendRow()
{
    // never append
    return false;
}

bool HaTable::DeleteRow([[maybe_unused]] size_t pos)
{
    if (m_index[pos].GetType() == HaTableIndex::ITEM) {
        m_doc->DeleteItem(m_index[pos].GetSegment(), m_index[pos].GetItem());
        m_index.erase(std::next(m_index.begin(), pos));
        for (auto p = pos; p < m_index.size() && m_index[p].GetType() != HaTableIndex::SEGMENT; ++p) {
            --m_index[p].GetSeq();
        }
        return true;
    }
    // do not delete
    return false;
}
