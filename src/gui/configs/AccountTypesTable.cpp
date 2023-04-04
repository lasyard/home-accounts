#include "AccountTypesTable.h"
#include "../Configs.h"
#include "ConfigsGridCellAttrProvider.h"

const wxString AccountTypesTable::LABEL = _("Account Types");
const wxString AccountTypesTable::COL_LABELS[] = {
    _("ID"),
    _("Name"),
};

AccountTypesTable::AccountTypesTable(CsvVecDao<struct account_type> *dao)
    : CsvTable(Configs::ACCOUNT_TYPES_SECTION_NAME, sizeof(COL_LABELS) / sizeof(wxString), COL_LABELS, dao)
{
    SetAttrProvider(new ConfigsGridCellAttrProvider(this));
}

AccountTypesTable::~AccountTypesTable()
{
}
