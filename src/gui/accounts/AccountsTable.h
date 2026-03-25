#ifndef _HA_ACCOUNTS_ACCOUNTS_TABLE_H_
#define _HA_ACCOUNTS_ACCOUNTS_TABLE_H_

#include "../HaTable.h"

class AccountsDoc;

class AccountsTable : public HaTable
{
    DECLARE_DYNAMIC_CLASS(AccountsTable)

public:
    static constexpr int ID_COL = 0;
    static constexpr int NAME_COL = 1;
    static constexpr int BANK_COL = 2;
    static constexpr int OPEN_DATE_COL = 3;
    static constexpr int INITIAL_COL = 4;
    static constexpr int MEMO_COL = 5;

    AccountsTable(AccountsDoc *doc = nullptr);
    virtual ~AccountsTable();

    void Init() override;
};

#endif /* _HA_ACCOUNTS_ACCOUNTS_TABLE_H_ */
