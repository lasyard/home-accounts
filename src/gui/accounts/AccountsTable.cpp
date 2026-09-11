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
    SetColImplDoc(_("ID"), ID_COL, AccountsDoc::ID_COL, true);
    SetColImplDoc(_("Name"), NAME_COL, AccountsDoc::NAME_COL);
    SetColImplDoc(_("Initial"), INITIAL_COL, AccountsDoc::INITIAL_COL);
    m_cache.resize(m_doc->GetRowCount());
}
