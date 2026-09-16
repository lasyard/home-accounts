#include <wx/log.h>

#include <stdexcept>

#include "HaTable.h"

#include "HaGridCellAttrProvider.h"

HaTable::HaTable(HaCsv *doc) : wxGridTableBase(), m_doc(doc), m_cache(), m_headerImpls(), m_colImpls()
{
}

HaTable::~HaTable()
{
    if (m_doc != nullptr) {
        delete m_doc;
    }
}

void HaTable::Prepare()
{
    auto *doc = GetDoc();
    wxASSERT(doc != nullptr);
    int cols = m_doc->GetColCount();
    m_colImpls.resize(cols);
    for (int i = 0; i < cols; ++i) {
        SetColImplDoc(i, i, true);
    }
    m_cache.resize(m_doc->GetRowCount());
}

void HaTable::Init()
{
    Prepare();
    for (auto i = 0; i < GetRowsCount(); ++i) {
        CacheRow(i);
    }
    if (GetAttrProvider() == nullptr) {
        SetAttrProvider(new HaGridCellAttrProvider(this));
    }
}

int HaTable::GetNumberRows()
{
    return m_cache.size();
}

int HaTable::GetNumberCols()
{
    return m_colImpls.size();
}

enum column_type HaTable::GetColType(int col) const
{
    return col < GetColsCount() ? m_colImpls[col].type : CT_IGNORE;
}

bool HaTable::IsColReadOnly(int col) const
{
    return m_colImpls[col].set == nullptr;
}

wxString HaTable::GetColLabelValue(int col)
{
    return m_colImpls[col].label;
}

wxString HaTable::GetRowLabelValue(int row)
{
    if ((size_t)row < m_headerImpls.size()) {
        return m_headerImpls[row].label;
    } else if (row < GetRowsCount()) {
        return wxString::Format("%d", row + 1 - (int)m_headerImpls.size());
    }
    return wxEmptyString;
}

record_t *HaTable::GetRowRecord(int row) const
{
    auto headers = m_headerImpls.size();
    return ((size_t)row >= headers && row < GetRowsCount()) ? m_doc->GetRecord(row - headers) : nullptr;
}

bool HaTable::CanHaveAttributes()
{
    return true;
}

wxString HaTable::GetValue(int row, int col)
{
    return m_cache[row][col];
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
        m_cache.insert(std::next(m_cache.begin(), pos), wxArrayString());
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
        m_cache.push_back(wxArrayString());
        CacheRow(m_cache.size() - 1);
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
    m_cache.erase(std::next(m_cache.begin(), pos), std::next(m_cache.begin(), pos + i));
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

const wxString HaTable::DocGetter(const HaTable *table, int row, int col)
{
    return table->m_doc->GetValueString(row, col);
}

void HaTable::DocSetter(HaTable *table, int row, int col, const wxString &value)
{
    table->m_doc->SetValueString(row, col, value);
}

wxString HaTable::GetHashString(int row) const
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

void HaTable::OnNewRow([[maybe_unused]] size_t pos)
{
}

const wxString HaTable::GetCellValue(int row, int col) const
{
    auto headers = m_headerImpls.size();
    if ((size_t)row < headers) {
        auto &impl = m_headerImpls[row];
        if (impl.get != nullptr) {
            return impl.get(this, impl.pos, col);
        }
    } else {
        auto flag = GetRowRecordFlag(row);
        if (flag == RECORD_FLAG_HASH) {
            if (col == 0) {
                return GetHashString(row);
            }
        } else if (col < GetColsCount()) {
            auto &impl = m_colImpls[col];
            if (impl.get != nullptr) {
                return impl.get(this, row - headers, impl.pos);
            }
            return _("not implemented");
        }
    }
    return wxEmptyString;
}

void HaTable::SetCellValue(int row, int col, const wxString &value)
{
    auto headers = m_headerImpls.size();
    if ((size_t)row < headers) {
        auto &impl = m_headerImpls[row];
        if (impl.set != nullptr) {
            impl.set(this, impl.pos, col, value);
        }
    } else {
        wxASSERT(GetRowRecordFlag(row) == RECORD_FLAG_NORMAL);
        if (col < GetColsCount() && m_colImpls[col].set != nullptr) {
            auto &impl = m_colImpls[col];
            impl.set(this, row - headers, impl.pos, value);
        }
    }
}
