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
    m_colImpls.resize(COLS);
    SetColImpl(_("ID"), ID_COL, AccountsDoc::ID_COL, true);
    SetColImpl(_("Name"), NAME_COL, AccountsDoc::NAME_COL);
    SetColImpl(_("Bank"), BANK_COL, AccountsDoc::BANK_COL);
    SetColImpl(_("Open Date"), OPEN_DATE_COL, AccountsDoc::OPEN_DATE_COL);
    SetColImpl(_("Initial"), INITIAL_COL, AccountsDoc::INITIAL_COL);
    SetColImpl(_("Memo"), MEMO_COL, AccountsDoc::MEMO_COL);
    m_cache.resize(m_doc->GetRowCount());
    HaTable::Init();
}
