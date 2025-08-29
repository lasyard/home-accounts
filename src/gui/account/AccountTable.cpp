#include "AccountTable.h"

#include "AccountDoc.h"
#include "AccountGridCellAttrProvider.h"

#include "../Utils.h"

#include "data/account.h"

IMPLEMENT_DYNAMIC_CLASS(AccountTable, HaImplTable)

AccountTable::AccountTable(AccountDoc *doc)
    : HaImplTable(
          5,
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
    MapColImpl(m_colImpls[0], ACCOUNT_ID_COL);
    MapColImpl(m_colImpls[1], ACCOUNT_NAME_COL);
    MapColImpl(m_colImpls[2], ACCOUNT_BEGIN_COL);
    MapColImpl(m_colImpls[3], ACCOUNT_INITIAL_COL);
    MapColImpl(m_colImpls[4], ACCOUNT_MEMO_COL);
}

AccountTable::~AccountTable()
{
}

AccountDoc *AccountTable::GetAccountDoc()
{
    return static_cast<AccountDoc *>(m_doc);
}

void AccountTable::Init()
{
    HaImplTable::Init();
    SetAttrProvider(new AccountGridCellAttrProvider(this));
}
