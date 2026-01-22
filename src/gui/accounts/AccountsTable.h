#ifndef _HA_ACCOUNTS_ACCOUNTS_TABLE_H_
#define _HA_ACCOUNTS_ACCOUNTS_TABLE_H_

#include "../HaTable.h"

class AccountsDoc;

class AccountsTable : public HaTable
{
    DECLARE_DYNAMIC_CLASS(AccountTable)

public:
    static const int ID_COL = 0;
    static const int NAME_COL = 1;

    AccountsTable(AccountsDoc *doc = nullptr);
    virtual ~AccountsTable();

    AccountsDoc *GetAccountDoc();
};

#endif /* _HA_ACCOUNTS_ACCOUNTS_TABLE_H_ */
