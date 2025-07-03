#ifndef _HA_ACCOUNT_ACCOUNT_TABLE_H_
#define _HA_ACCOUNT_ACCOUNT_TABLE_H_

#include "../HaImplTable.h"

class AccountDoc;

class AccountTable : public HaImplTable
{
    DECLARE_DYNAMIC_CLASS(AccountTable)

public:
    AccountTable(AccountDoc *doc = nullptr);
    virtual ~AccountTable();

    AccountDoc *GetAccountDoc();
};

#endif /* _HA_ACCOUNT_ACCOUNT_TABLE_H_ */
