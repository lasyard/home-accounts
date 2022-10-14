#ifndef _HA_DATA_TABLE_H_
#define _HA_DATA_TABLE_H_

#include <wx/grid.h>

#include "DataTableCellAttrs.h"
#include "csv/ColumnType.h"
#include "data/DataFile.h"

class DataTable : public wxGridTableBase
{
public:
    DataTable(DataFile *dataFile);

    virtual ~DataTable();

    int GetNumberRows() override
    {
        return m_dataFile->getNumberRows();
    }

    int GetNumberCols() override
    {
        return m_dataFile->getNumberCols();
    }

    wxString GetValue(int row, int col) override;
    wxString GetRowLabelValue(int row) override;
    wxString GetColLabelValue(int col) override;

    void SetValue(int row, int col, const wxString &value) override;

    bool CanHaveAttributes() override
    {
        return true;
    }

    wxGridCellAttr *GetAttr(int row, int col, wxGridCellAttr::wxAttrKind kind) override;

private:
    DataFile *m_dataFile;
    DataTableCellAttrs m_attrs;
};

#endif /* _HA_DATA_TABLE_H_ */
