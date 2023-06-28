#include "AccountTypesTable.h"

#include "ConfigsGridCellAttrProvider.h"

const wxString AccountTypesTable::COL_LABELS[] = {
    t("ID"),
    t("Name"),
};

AccountTypesTable::AccountTypesTable(CsvVecDao<struct account_type> *dao)
    : CsvTable(sizeof(COL_LABELS) / sizeof(wxString), COL_LABELS, dao)
{
    SetAttrProvider(new ConfigsGridCellAttrProvider(this));
}

AccountTypesTable::~AccountTypesTable()
{
}
