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

private:
    static constexpr int HEADER_ROWS = 1;

    wxString HeaderGetter(int row, int col) const;
    void HeaderSetter(int row, int col, const wxString &value);
};

#endif /* _HA_IMPORT_IMPORT_TABLE_H_ */
