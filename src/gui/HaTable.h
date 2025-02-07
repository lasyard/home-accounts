#ifndef _HA_GUI_HA_TABLE_H_
#define _HA_GUI_HA_TABLE_H_

#include <wx/grid.h>
#include <wx/vector.h>

#include "CsvDoc.h"

#include "csv/column_type.h"

class HaTable : public wxGridTableBase
{
public:
    HaTable(CsvDoc *doc);
    virtual ~HaTable();

    int GetNumberRows() override;
    int GetNumberCols() override;

    wxString GetValue(int row, int col) override;
    wxString GetColLabelValue(int col) override;
    wxString GetRowLabelValue(int row) override;

    void SetValue(int row, int col, const wxString &value) override;

    bool InsertRows(size_t pos, size_t numRows) override;
    bool AppendRows(size_t numRows) override;
    bool DeleteRows(size_t pos, size_t numRows) override;

    bool CanHaveAttributes() override;

    enum column_type GetItemFieldType(int col) const
    {
        return m_doc->GetItemValueType(col);
    }

    auto *GetCsvDoc()
    {
        return m_doc;
    }

protected:
    CsvDoc *m_doc;
    wxVector<wxArrayString> *m_cache;
    wxArrayString m_colLabels;

    void CacheCell(int row, int col);
    void CacheRow(int row);
    void CacheCol(int col);

    /**
     * @brief Refresh contents of a grid column and auto resize it to fit the contents.
     *
     * @param col the column
     */
    void RefreshAndAutoSizeGridColumn(int col);

    virtual const wxString GetCellValue(int row, int col) = 0;
    virtual void SetCellValue(int row, int col, const wxString &value) = 0;

    virtual bool InsertRow(size_t pos) = 0;
    virtual bool AppendRow() = 0;
    virtual bool DeleteRow(size_t pos) = 0;
};

#endif /* _HA_GUI_HA_TABLE_H_ */
