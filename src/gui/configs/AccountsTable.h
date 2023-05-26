#ifndef _CONFIGS_ACCOUNTS_TABLE_H_
#define _CONFIGS_ACCOUNTS_TABLE_H_

#include "../CsvTable.h"

#include "data/AccountsDao.h"
#include "data/ConfigPodsTraits.h"

class AccountsTable : public CsvTable<struct account>
{
public:
    static const wxString COL_LABELS[];

    AccountsTable(AccountsDao *dao);
    virtual ~AccountsTable();

    void UpdateChoicesFromJoints();

    AccountsTable *Clone() const override
    {
        return new AccountsTable(dynamic_cast<AccountsDao *>(m_dao));
    }
};

#endif /* _CONFIGS_ACCOUNTS_TABLE_H_ */
