#include <wx/log.h>

#include "DataGridCellAttrProvider.h"

IMPLEMENT_TM(DataGridCellAttrProvider)

DataGridCellAttrProvider::DataGridCellAttrProvider(HaTable *table) : HaGridCellAttrProvider(table)
{
    wxLog::AddTraceMask(TM);
    auto *renderer = new wxGridCellDateRenderer(_("%Y-%m-%d"));
    m_commentAttrRO->SetRenderer(renderer);
}

DataGridCellAttrProvider::~DataGridCellAttrProvider()
{
}
