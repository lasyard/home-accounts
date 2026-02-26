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
    int cols = m_doc != nullptr ? m_doc->GetColCount() : 0;
    m_colLabels.clear();
    for (int i = 0; i < cols; ++i) {
        m_colLabels.Add(wxString::Format("Col%d", i + 1));
    }
    HaTable::Init();
}
