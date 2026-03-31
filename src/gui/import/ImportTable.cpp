#include "ImportTable.h"

#include "ImportDoc.h"
#include "ImportGridCellAttrProvider.h"

#include "../data/DataDoc.h"

IMPLEMENT_DYNAMIC_CLASS(ImportTable, HaTable)

ImportTable::ImportTable(ImportDoc *doc) : HaTableTemplate<ImportTable, ImportDoc>(doc)
{
}

ImportTable::~ImportTable()
{
}

void ImportTable::Init()
{
    auto *doc = GetDoc();
    wxASSERT(doc != nullptr);
    m_headerImpls.resize(HEADER_ROWS);
    SetHeaderImpl(_("Field"), 0, CT_STR, &ImportTable::HeaderGetter, &ImportTable::HeaderSetter);
    int cols = m_doc->GetColCount();
    m_colImpls.resize(cols);
    for (int i = 0; i < cols; ++i) {
        SetColImplDoc(static_cast<ImportDoc *>(m_doc)->GetCsvTitle(i), i, i, true);
    }
    m_cache.resize(m_doc->GetRowCount() + HEADER_ROWS);
    wxArrayString fieldNames;
    for (int i = -1; i < DataDoc::COLS; ++i) {
        fieldNames.Add(DataDoc::GetColName(i));
    }
    SetAttrProvider(new ImportGridCellAttrProvider(this, fieldNames));
    HaTable::Init();
}

bool ImportTable::IsInvalidCol(int col) const
{
    auto *doc = GetDoc();
    wxASSERT(doc != nullptr && col >= 0 && col < doc->GetColCount());
    return doc->GetDataField(col) < 0;
}

wxString ImportTable::HeaderGetter([[maybe_unused]] int row, int col) const
{
    return GetDoc()->GetDataFieldName(col);
}

void ImportTable::HeaderSetter([[maybe_unused]] int row, int col, const wxString &value)
{
    GetDoc()->SetDataFieldByName(col, value);
}
