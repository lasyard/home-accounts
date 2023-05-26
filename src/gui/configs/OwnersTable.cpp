#include "OwnersTable.h"

#include "ConfigsGridCellAttrProvider.h"

const wxString OwnersTable::COL_LABELS[] = {
    _("ID"),
    _("Name"),
};

OwnersTable::OwnersTable(CsvVecDao<struct owner> *dao)
    : CsvTable(sizeof(COL_LABELS) / sizeof(wxString), COL_LABELS, dao)
{
    SetAttrProvider(new ConfigsGridCellAttrProvider(this));
}

OwnersTable::~OwnersTable()
{
}
