#include "HaDataTable.h"

#include "HaDataGridCellAttrProvider.h"

IMPLEMENT_DYNAMIC_CLASS(HaDataTable, HaTable)

HaDataTable::HaDataTable(CsvDoc *doc) : HaTable(doc)
{
    SetAttrProvider(new HaDataGridCellAttrProvider(this));
}

HaDataTable::~HaDataTable()
{
}
