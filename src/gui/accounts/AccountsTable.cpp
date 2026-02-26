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
    MapColToCol(BANK_COL, AccountsDoc::BANK_COL);
    MapColToCol(OPEN_DATE_COL, AccountsDoc::OPEN_DATE_COL);
    MapColToCol(INITIAL_COL, AccountsDoc::INITIAL_COL);
    MapColToCol(MEMO_COL, AccountsDoc::MEMO_COL);
}

AccountsTable::~AccountsTable()
{
}

AccountsDoc *AccountsTable::GetAccountDoc()
{
    return static_cast<AccountsDoc *>(m_doc);
}
