#include "AccountsTable.h"

#include "AccountsDoc.h"

IMPLEMENT_DYNAMIC_CLASS(AccountsTable, HaTable)

AccountsTable::AccountsTable(AccountsDoc *doc) : HaTable(doc)
{
}

AccountsTable::~AccountsTable()
{
}

void AccountsTable::Prepare()
{
    m_colImpls.resize(COLS);
    SetColImplDoc(ID_COL, AccountsDoc::ID_COL, true);
    SetColImplDoc(NAME_COL, AccountsDoc::NAME_COL);
    m_cache.resize(m_doc->GetRowCount());
}
