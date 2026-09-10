#include "ImportTable.h"

#include "ImportDoc.h"

IMPLEMENT_DYNAMIC_CLASS(ImportTable, HaTable)

ImportTable::ImportTable(ImportDoc *doc) : HaTable(doc)
{
}

ImportTable::~ImportTable()
{
}
