#ifndef _CONFIGS_ACCOUNT_TYPES_TABLE_H_
#define _CONFIGS_ACCOUNT_TYPES_TABLE_H_

#include "../CsvTable.h"

#include "data/ConfigPodsTraits.h"

class AccountTypesTable : public CsvTable<struct account_type>
{
public:
    static const wxString COL_LABELS[];

    AccountTypesTable(CsvVecDao<struct account_type> *dao);
    virtual ~AccountTypesTable();

    AccountTypesTable *Clone() const override
    {
        return new AccountTypesTable(m_dao);
    }
};

#endif /* _CONFIGS_ACCOUNT_TYPES_TABLE_H_ */
