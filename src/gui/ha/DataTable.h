#ifndef _HA_DATA_TABLE_H_
#define _HA_DATA_TABLE_H_

#include <wx/grid.h>

class DataTable : public wxGridTableBase
{
public:
    DataTable() : wxGridTableBase()
    {
    }

    virtual ~DataTable()
    {
    }

    int GetNumberRows() override
    {
        return 100;
    }

    int GetNumberCols() override
    {
        return 10;
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
    wxString m_value;
};

#endif /* _HA_DATA_TABLE_H_ */
