#include <wx/log.h>

#include "HaDataGridCellAttrProvider.h"

IMPLEMENT_TM(HaDataGridCellAttrProvider)

HaDataGridCellAttrProvider::HaDataGridCellAttrProvider(const HaTable *table) : HaGridCellAttrProvider(table)
{
    wxLog::AddTraceMask(TM);
}

HaDataGridCellAttrProvider::~HaDataGridCellAttrProvider()
{
}
