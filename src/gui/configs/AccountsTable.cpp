#include "AccountsTable.h"
#include "../Configs.h"
#include "AccountsGridCellAttrProvider.h"

const wxString AccountsTable::LABEL = _("Accounts");
const wxString AccountsTable::COL_LABELS[] = {
    _("ID"),
    _("Name"),
    _("Owner"),
    _("Type"),
    _("Description"),
    _("Balance"),
};

AccountsTable::AccountsTable(AccountsDao *dao)
    : CsvTable(Configs::ACCOUNTS_SECTION_NAME, sizeof(COL_LABELS) / sizeof(wxString), COL_LABELS, dao)
{
    SetAttrProvider(new AccountsGridCellAttrProvider(this));
}

AccountsTable::~AccountsTable()
{
}

void AccountsTable::SetOwnerChoices(wxArrayString &choices)
{
    static_cast<AccountsGridCellAttrProvider *>(GetAttrProvider())->SetOwnerChoices(choices);
}

void AccountsTable::SetTypeChoices(wxArrayString &choices)
{
    static_cast<AccountsGridCellAttrProvider *>(GetAttrProvider())->SetTypeChoices(choices);
}
