#include "ImportTable.h"

#include "ImportDoc.h"
#include "ImportGridCellAttrProvider.h"

IMPLEMENT_DYNAMIC_CLASS(ImportTable, HaTable)

ImportTable::ImportTable(ImportDoc *doc) : HaTable(doc)
{
}

ImportTable::~ImportTable()
{
}

void ImportTable::Init()
{
    wxASSERT(m_doc != nullptr);
    int cols = m_doc->GetColCount();
    for (int i = 0; i < cols; ++i) {
        m_colLabels.Add(static_cast<ImportDoc *>(m_doc)->GetCsvTitle(i));
    }
    m_colImpls = new struct ColImpl[m_colLabels.size()];
    for (int i = 0; i < cols; ++i) {
        MapColToCol(i, i, true);
    }
    HaTable::Init();
    SetAttrProvider(new ImportGridCellAttrProvider(this));
}

int ImportTable::GetNumberRows()
{
    return HaTable::GetNumberRows() + HEADER_ROWS;
}

wxString ImportTable::GetValue(int row, int col)
{
    auto *doc = static_cast<ImportDoc *>(m_doc);
    wxASSERT(doc != nullptr);
    if (row == 0) {
        return doc->GetDataFieldName(col);
    }
    return HaTable::GetValue(row - HEADER_ROWS, col);
}

wxString ImportTable::GetRowLabelValue(int row)
{
    if (row == 0) {
        return _("Field");
    }
    return wxString::Format("%d", row - HEADER_ROWS);
}

record_t *ImportTable::GetRowRecord(int row) const
{
    if (row != 0) {
        return HaTable::GetRowRecord(row - HEADER_ROWS);
    }
    return nullptr;
}

bool ImportTable::IsInvalidCol(int col) const
{
    auto *doc = static_cast<ImportDoc *>(m_doc);
    wxASSERT(doc != nullptr && col >= 0 && col < doc->GetColCount());
    return doc->GetDataField(col) < 0;
}
