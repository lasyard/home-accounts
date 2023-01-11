#ifndef _HA_DATA_TABLE_H_
#define _HA_DATA_TABLE_H_

#include <wx/grid.h>
#include <wx/vector.h>

#include "DataTableCellAttrs.h"
#include "csv/ColumnType.h"
#include "data/DataFile.h"

class DataTable : public wxGridTableBase
{
public:
    static const int TIME_COL = 0;
    static const int INCOME_COL = 1;
    static const int OUTLAY_COL = 2;
    static const int DESC_COL = 3;
    static const int COL_NUM = 4;
    static const wxString COL_LABELS[];

    DataTable(DataFile *dataFile);

    virtual ~DataTable();

    int GetNumberRows() override
    {
        return m_dataFile->getNumberRows();
    }

    int GetNumberCols() override
    {
        return COL_NUM;
    }

    wxString GetValue(int row, int col) override;
    wxString GetRowLabelValue(int row) override;
    wxString GetColLabelValue(int col) override;

    void SetValue(int row, int col, const wxString &value) override;

    bool InsertRows(size_t pos, size_t numRows) override;

    bool CanHaveAttributes() override
    {
        return true;
    }

    wxGridCellAttr *GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) override;

private:
    DataFile *m_dataFile;
    DataTableCellAttrs m_attrs;
    wxVector<wxArrayString> m_cache;

    void CacheRow(int row);

    wxString GetCellValue(int row, int col);

    void SetCellValue(int row, int col, const std::string &value);
};

#endif /* _HA_DATA_TABLE_H_ */
