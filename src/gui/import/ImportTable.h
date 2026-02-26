#ifndef _HA_IMPORT_IMPORT_TABLE_H_
#define _HA_IMPORT_IMPORT_TABLE_H_

#include "../HaTable.h"

class ImportDoc;

class ImportTable : public HaTable
{
    DECLARE_DYNAMIC_CLASS(ImportTable)

public:
    ImportTable(ImportDoc *doc = nullptr);
    virtual ~ImportTable();

    void Init() override;
};

#endif /* _HA_IMPORT_IMPORT_TABLE_H_ */
