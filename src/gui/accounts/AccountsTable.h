#ifndef _HA_ACCOUNTS_ACCOUNTS_TABLE_H_
#define _HA_ACCOUNTS_ACCOUNTS_TABLE_H_

#include "../HaTable.h"

class AccountsDoc;

class AccountsTable : public HaTable
{
    DECLARE_DYNAMIC_CLASS(AccountsTable)

public:
    static const int ID_COL = 0;
    static const int NAME_COL = 1;
    static const int BANK_COL = 2;
    static const int OPEN_DATE_COL = 3;
    static const int INITIAL_COL = 4;
    static const int MEMO_COL = 5;

    AccountsTable(AccountsDoc *doc = nullptr);
    virtual ~AccountsTable();

    void Init() override;
};

#endif /* _HA_ACCOUNTS_ACCOUNTS_TABLE_H_ */
