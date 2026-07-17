#ifndef _HA_IMPORT_IMPORT_TABLE_H_
#define _HA_IMPORT_IMPORT_TABLE_H_

#include "../HaTableTemplate.h"

class ImportDoc;

class ImportTable : public HaTableTemplate<ImportTable, ImportDoc>
{
    DECLARE_DYNAMIC_CLASS(ImportTable)

public:
    ImportTable(ImportDoc *doc = nullptr);
    virtual ~ImportTable();

    void Init() override;

    bool IsInvalidCol(int col) const;
};

#endif /* _HA_IMPORT_IMPORT_TABLE_H_ */
