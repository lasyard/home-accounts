#include "AccountsTable.h"

#include "AccountsDoc.h"

IMPLEMENT_DYNAMIC_CLASS(AccountsTable, HaTable)

AccountsTable::AccountsTable(AccountsDoc *doc) : HaTableTemplate<AccountsTable, AccountsDoc>(doc)
{
}

AccountsTable::~AccountsTable()
{
}

void AccountsTable::Init()
{
    m_colImpls.resize(COLS);
    SetColImplDoc(_("ID"), ID_COL, AccountsDoc::ID_COL, true);
    SetColImplDoc(_("Name"), NAME_COL, AccountsDoc::NAME_COL);
    SetColImplDoc(_("Bank"), BANK_COL, AccountsDoc::BANK_COL);
    SetColImplDoc(_("Open Date"), OPEN_DATE_COL, AccountsDoc::OPEN_DATE_COL);
    SetColImplDoc(_("Initial"), INITIAL_COL, AccountsDoc::INITIAL_COL);
    SetColImplDoc(_("Memo"), MEMO_COL, AccountsDoc::MEMO_COL);
    m_cache.resize(m_doc->GetRowCount());
    HaTable::Init();
}
