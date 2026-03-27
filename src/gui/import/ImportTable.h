#ifndef _HA_IMPORT_IMPORT_TABLE_H_
#define _HA_IMPORT_IMPORT_TABLE_H_

#include "../HaTableTemplate.h"

class ImportDoc;

class ImportTable : public HaTableTemplate<ImportDoc>
{
    DECLARE_DYNAMIC_CLASS(ImportTable)

public:
    ImportTable(ImportDoc *doc = nullptr);
    virtual ~ImportTable();

    void Init() override;

    bool IsInvalidCol(int col) const;

private:
    static constexpr int HEADER_ROWS = 1;
};

#endif /* _HA_IMPORT_IMPORT_TABLE_H_ */
