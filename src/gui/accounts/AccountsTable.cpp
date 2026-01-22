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
    MapColToCol(ID_COL, AccountsDoc::ID_COL, true);
    MapColToCol(NAME_COL, AccountsDoc::NAME_COL);
    MapColToCol(2, AccountsDoc::BANK_COL);
    MapColToCol(3, AccountsDoc::OPEN_DATE_COL);
    MapColToCol(4, AccountsDoc::INITIAL_COL);
    MapColToCol(5, AccountsDoc::MEMO_COL);
}

AccountsTable::~AccountsTable()
{
}

AccountsDoc *AccountsTable::GetAccountDoc()
{
    return static_cast<AccountsDoc *>(m_doc);
}
