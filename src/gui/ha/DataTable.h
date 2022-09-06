#ifndef _HA_DATA_TABLE_H_
#define _HA_DATA_TABLE_H_

#include <wx/grid.h>

#include "csv/ColumnType.h"

class DataTable : public wxGridTableBase
{
public:
    DataTable(ColumnType types[], int cols);

    virtual ~DataTable();

    int GetNumberRows() override
    {
        return m_rows;
    }

    int GetNumberCols() override
    {
        return m_cols;
    }

    wxString GetValue(int row, int col) override;
    wxString GetRowLabelValue(int row) override;
    wxString GetColLabelValue(int col) override;

    void SetValue(int row, int col, const wxString &value) override;

    bool CanHaveAttributes() override
    {
        return false;
    }

private:
    int m_cols;
    int m_rows;
    ColumnType *m_types;
    void **m_values;

    void AllocateVectors();
    void ReleaseVectors();
};

#endif /* _HA_DATA_TABLE_H_ */
