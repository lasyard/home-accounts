#include "ImportTable.h"

#include "ImportDoc.h"
#include "ImportGridCellAttrProvider.h"

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
    int cols = m_doc->GetColCount();
    m_colImpls.resize(cols);
    for (int i = 0; i < cols; ++i) {
        SetColImplDoc(static_cast<ImportDoc *>(m_doc)->GetCsvTitle(i), i, i, true);
    }
    m_cache.resize(m_doc->GetRowCount());
    SetAttrProvider(new ImportGridCellAttrProvider(this));
    HaTable::Init();
}

bool ImportTable::IsInvalidCol(int col) const
{
    auto *doc = GetDoc();
    wxASSERT(doc != nullptr && col >= 0 && col < doc->GetColCount());
    return doc->GetDataField(col) < 0;
}
