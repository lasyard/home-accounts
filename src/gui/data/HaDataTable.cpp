#include "HaDataTable.h"

#include "HaDataGridCellAttrProvider.h"

#include "../CsvDoc.h"

IMPLEMENT_DYNAMIC_CLASS(HaDataTable, HaTable)

HaDataTable::HaDataTable(CsvDoc *doc) : HaTable(doc)
{
    CreateIndexAndCache();
    SetAttrProvider(new HaDataGridCellAttrProvider(this));
}

HaDataTable::~HaDataTable()
{
}

wxString HaDataTable::GetRowLabelValue(int row)
{
    if ((size_t)row < m_index.size()) {
        const struct IndexItem &item = m_index[row];
        if (item.m_type == ITEM) {
            return wxString::Format("%d", item.m_seq);
        }
    }
    return wxEmptyString;
}

void HaDataTable::CreateIndexAndCache()
{
    m_index.clear();
    if (m_doc != nullptr) {
        m_doc->ForEachSegment([this](struct segment *segment) -> bool {
            m_index.push_back(IndexItem(segment));
            int seq = 1;
            m_doc->ForEachItem(segment, [this, &seq](void *item) -> bool {
                m_index.push_back(IndexItem(item, seq++));
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
}

const wxString HaDataTable::GetCellValue(int row, int col)
{
    switch (GetRowType(row)) {
    case ITEM:
        if ((size_t)col < m_colLabels.size()) {
            return m_doc->GetItemValueString(m_index[row].m_ptr, col);
        }
        break;
    case SEGMENT:
        if (col == 0) {
            return m_doc->GetSegmentValueString(static_cast<struct segment *>(m_index[row].m_ptr));
        }
        break;
    case OTHER:
        break;
    }
    return "";
}

void HaDataTable::SetCellValue(int row, int col, const wxString &value)
{
    switch (GetRowType(row)) {
    case ITEM:
        if ((size_t)col < m_colLabels.size()) {
            m_doc->SetItemValueString(static_cast<struct item *>(m_index[row].m_ptr), col, value);
        }
        break;
    case SEGMENT:
        if (col == 0) {
            m_doc->SetSegmentValueString(static_cast<struct segment *>(m_index[row].m_ptr), value);
        }
        break;
    case OTHER:
        break;
    }
}

bool HaDataTable::InsertRow(size_t pos)
{
    auto &index = m_index[pos];
    void *item = nullptr;
    if (index.m_type == ITEM) {
        item = m_doc->InsertItem(index.m_ptr);
    } else if (index.m_type == SEGMENT) {
        item = m_doc->InsertItemHead(static_cast<struct segment *>(index.m_ptr));
    }
    if (item != nullptr) {
        int seq = index.m_seq + 1;
        m_index.insert(std::next(m_index.begin(), pos + 1), IndexItem(item, seq));
        for (auto p = pos + 2; p < m_index.size() && m_index[p].m_type != SEGMENT; ++p) {
            ++m_index[p].m_seq;
        }
        return true;
    }
    return false;
}

bool HaDataTable::AppendRow()
{
    // never append
    return false;
}

bool HaDataTable::DeleteRow([[maybe_unused]] size_t pos)
{
    if (m_index[pos].m_type == ITEM) {
        void *item = m_index[pos].m_ptr;
        size_t sp;
        for (sp = pos - 1; m_index[sp].m_type != SEGMENT; --sp)
            ;
        struct segment *segment = (struct segment *)m_index[sp].m_ptr;
        m_doc->DeleteItem(segment, item);
        m_index.erase(std::next(m_index.begin(), pos));
        for (auto p = pos; p < m_index.size() && m_index[p].m_type != SEGMENT; ++p) {
            --m_index[p].m_seq;
        }
        return true;
    }
    // do not delete
    return false;
}
