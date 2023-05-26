#ifndef _CONFIGS_OWNER_TABLE_H_
#define _CONFIGS_OWNER_TABLE_H_

#include "../CsvTable.h"

#include "data/ConfigPodsTraits.h"

class OwnersTable : public CsvTable<struct owner>
{
public:
    static const wxString COL_LABELS[];

    OwnersTable(CsvVecDao<struct owner> *dao);
    virtual ~OwnersTable();

    OwnersTable *Clone() const override
    {
        return new OwnersTable(m_dao);
    }
};

#endif /* _CONFIGS_OWNER_TABLE_H_ */
