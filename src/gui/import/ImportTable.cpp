#include "ImportTable.h"

#include "ImportDoc.h"

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
        m_colLabels.Add(wxString::Format("%d", i + 1));
    }
    m_colImpls = new struct ColImpl[m_colLabels.size()];
    for (int i = 0; i < cols; ++i) {
        MapColToCol(i, i);
    }
    HaTable::Init();
}
