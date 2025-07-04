#ifndef _HA_ACCOUNT_ACCOUNT_TABLE_H_
#define _HA_ACCOUNT_ACCOUNT_TABLE_H_

#include "../HaImplTable.h"

class AccountDoc;

class AccountTable : public HaImplTable
{
    DECLARE_DYNAMIC_CLASS(AccountTable)

public:
    static const int ID_COL = 0;

    AccountTable(AccountDoc *doc = nullptr);
    virtual ~AccountTable();

    AccountDoc *GetAccountDoc();

    void Init() override;
};

#endif /* _HA_ACCOUNT_ACCOUNT_TABLE_H_ */
