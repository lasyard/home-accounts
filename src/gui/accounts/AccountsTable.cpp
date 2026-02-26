#include "AccountsTable.h"

#include "AccountsDoc.h"

IMPLEMENT_DYNAMIC_CLASS(AccountsTable, HaTable)

AccountsTable::AccountsTable(AccountsDoc *doc) : HaTable(doc)
{
}

AccountsTable::~AccountsTable()
{
}

void AccountsTable::Init()
{
    m_colLabels = {
        _("ID"),
        _("Name"),
        _("Bank"),
        _("Open Date"),
        _("Initial"),
        _("Memo"),
    };
    m_colImpls = new struct ColImpl[m_colLabels.size()];
    MapColToCol(ID_COL, AccountsDoc::ID_COL, true);
    MapColToCol(NAME_COL, AccountsDoc::NAME_COL);
    MapColToCol(BANK_COL, AccountsDoc::BANK_COL);
    MapColToCol(OPEN_DATE_COL, AccountsDoc::OPEN_DATE_COL);
    MapColToCol(INITIAL_COL, AccountsDoc::INITIAL_COL);
    MapColToCol(MEMO_COL, AccountsDoc::MEMO_COL);
    HaTable::Init();
}
