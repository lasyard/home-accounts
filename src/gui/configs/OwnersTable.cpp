#include "OwnersTable.h"
#include "../Configs.h"
#include "ConfigsGridCellAttrProvider.h"

const wxString OwnersTable::LABEL = _("Owners");
const wxString OwnersTable::COL_LABELS[] = {
    _("ID"),
    _("Name"),
};

OwnersTable::OwnersTable(CsvVecDao<struct owner> *dao)
    : CsvTable(Configs::OWNERS_SECTION_NAME, sizeof(COL_LABELS) / sizeof(wxString), COL_LABELS, dao)
{
    SetAttrProvider(new ConfigsGridCellAttrProvider(this));
}

OwnersTable::~OwnersTable()
{
}
