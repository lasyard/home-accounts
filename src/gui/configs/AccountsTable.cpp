#include "AccountsTable.h"

#include "AccountsGridCellAttrProvider.h"

#include "../utils/DaoUtils.h"

const wxString AccountsTable::COL_LABELS[] = {
    t("ID"),
    t("Name"),
    t("Owner"),
    t("Type"),
    t("Description"),
    t("Balance"),
    t("Bill Config"),
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
    Utils::GetStrings(choices, dao->getOwnerJoint());
    static_cast<AccountsGridCellAttrProvider *>(GetAttrProvider())->SetOwnerChoices(choices);
    Utils::GetStrings(choices, dao->getTypeJoint());
    static_cast<AccountsGridCellAttrProvider *>(GetAttrProvider())->SetTypeChoices(choices);
}
