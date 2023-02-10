#ifndef _HA_DATA_TABLE_H_
#define _HA_DATA_TABLE_H_

#include "CachedTable.h"
#include "DataTableCellAttrs.h"

class DataDao;

class DataTable : public CachedTable
{
public:
    static const int TIME_COL = 0;
    static const int INCOME_COL = 1;
    static const int OUTLAY_COL = 2;
    static const int DESC_COL = 3;
    static const int COL_NUM = 4;

    static const wxString COL_LABELS[];

    DataTable(DataDao *dataDao);

    virtual ~DataTable();

    wxString GetRowLabelValue(int row) override;

    wxGridCellAttr *GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) override;

private:
    DataDao *m_dataDao;
    DataTableCellAttrs m_attrs;

    wxString GetCellValue(int row, int col) override;

    void SetCellValue(int row, int col, const std::string &value) override;

    bool InsertRow(int pos) override;
};

#endif /* _HA_DATA_TABLE_H_ */
