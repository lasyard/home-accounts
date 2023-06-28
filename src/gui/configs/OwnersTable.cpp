#include "OwnersTable.h"

#include "ConfigsGridCellAttrProvider.h"

const wxString OwnersTable::COL_LABELS[] = {
    t("ID"),
    t("Name"),
};

OwnersTable::OwnersTable(CsvVecDao<struct owner> *dao)
    : CsvTable(sizeof(COL_LABELS) / sizeof(wxString), COL_LABELS, dao)
{
    SetAttrProvider(new ConfigsGridCellAttrProvider(this));
}

OwnersTable::~OwnersTable()
{
}
