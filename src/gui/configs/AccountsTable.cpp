#include "AccountsTable.h"

#include "AccountsGridCellAttrProvider.h"

const wxString AccountsTable::COL_LABELS[] = {
    _("ID"),
    _("Name"),
    _("Owner"),
    _("Type"),
    _("Description"),
    _("Balance"),
};

AccountsTable::AccountsTable(AccountsDao *dao) : CsvTable(sizeof(COL_LABELS) / sizeof(wxString), COL_LABELS, dao)
{
    SetAttrProvider(new AccountsGridCellAttrProvider(this));
}

AccountsTable::~AccountsTable()
{
}

void AccountsTable::UpdateChoicesFromJoints()
{
    auto dao = static_cast<AccountsDao *>(m_dao);
    wxArrayString choices;
    Common::GetChoices(choices, dao->getOwnerJoint());
    static_cast<AccountsGridCellAttrProvider *>(GetAttrProvider())->SetOwnerChoices(choices);
    Common::GetChoices(choices, dao->getTypeJoint());
    static_cast<AccountsGridCellAttrProvider *>(GetAttrProvider())->SetTypeChoices(choices);
}
