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

    int GetNumberRows() override;
    wxString GetValue(int row, int col) override;
    wxString GetRowLabelValue(int row) override;
    record_t *GetRowRecord(int row) const override;

    bool IsInvalidCol(int col) const;

private:
    static constexpr int HEADER_ROWS = 1;
};

#endif /* _HA_IMPORT_IMPORT_TABLE_H_ */
