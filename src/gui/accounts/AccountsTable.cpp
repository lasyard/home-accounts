#include "AccountsTable.h"

#include "AccountsDoc.h"

IMPLEMENT_DYNAMIC_CLASS(AccountsTable, HaTable)

AccountsTable::AccountsTable(AccountsDoc *doc)
    : HaTable(
          {
              _("ID"),
              _("Name"),
              _("Bank"),
              _("Open Date"),
              _("Initial"),
              _("Memo"),
          },
          doc
      )
{
    MapColToCol(0, AccountsDoc::ACCOUNT_ID_COL, true);
    MapColToCol(1, AccountsDoc::ACCOUNT_NAME_COL);
    MapColToCol(2, AccountsDoc::ACCOUNT_BANK_COL);
    MapColToCol(3, AccountsDoc::ACCOUNT_OPEN_COL);
    MapColToCol(4, AccountsDoc::ACCOUNT_INITIAL_COL);
    MapColToCol(5, AccountsDoc::ACCOUNT_MEMO_COL);
}

AccountsTable::~AccountsTable()
{
}

AccountsDoc *AccountsTable::GetAccountDoc()
{
    return static_cast<AccountsDoc *>(m_doc);
}
