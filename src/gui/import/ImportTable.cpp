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
    m_headerImpls.resize(HEADER_ROWS);
    auto *doc = GetImportDoc();
    m_headerImpls[0] = {
        .label = _("Field"),
        .type = CT_STR,
        .get = [doc](int col) -> wxString { return doc->GetDataFieldName(col); },
        // doc will be reload so need not to refresh
        .set = [doc](int col, const wxString &value) -> void { doc->SetDataFieldByName(col, value); },
    };
    int cols = m_doc->GetColCount();
    m_colImpls.resize(cols);
    for (int i = 0; i < cols; ++i) {
        SetColImpl(static_cast<ImportDoc *>(m_doc)->GetCsvTitle(i), i, i, true);
    }
    m_cache.resize(m_doc->GetRowCount() + HEADER_ROWS);
    SetAttrProvider(new ImportGridCellAttrProvider(this));
    HaTable::Init();
}

bool ImportTable::IsInvalidCol(int col) const
{
    auto *doc = GetImportDoc();
    wxASSERT(doc != nullptr && col >= 0 && col < doc->GetColCount());
    return doc->GetDataField(col) < 0;
}

ImportDoc *ImportTable::GetImportDoc()
{
    return static_cast<ImportDoc *>(m_doc);
}
