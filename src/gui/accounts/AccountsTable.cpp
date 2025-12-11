#include "AccountsTable.h"

#include "AccountsDoc.h"

IMPLEMENT_DYNAMIC_CLASS(AccountsTable, HaTable)

AccountsTable::AccountsTable(AccountsDoc *doc)
    : HaTable(
          {
              _("ID"),
              _("Name"),
              _("Open Date"),
              _("Initial"),
              _("Memo"),
          },
          doc
      )
{
    MapAllColsToDoc();
}

AccountsTable::~AccountsTable()
{
}

AccountsDoc *AccountsTable::GetAccountDoc()
{
    return static_cast<AccountsDoc *>(m_doc);
}
