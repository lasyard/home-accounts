#ifndef _HA_GUI_HA_TABLE_H_
#define _HA_GUI_HA_TABLE_H_

#include <wx/grid.h>
#include <wx/vector.h>

#include "HaCsv.h"

class HaTable : public wxGridTableBase
{
public:
    HaTable(HaCsv *doc);
    virtual ~HaTable();

    virtual void Init();

    int GetNumberRows() override;

    virtual enum column_type GetColType(int col) const = 0;

    virtual bool IsColReadOnly(int col) const = 0;

    virtual record_t *GetRowRecord(int row) const = 0;

    auto GetRowRecordFlag(int row) const
    {
        record_t *record = GetRowRecord(row);
        return (record != nullptr) ? record->flag : RECORD_FLAG_INVALID;
    }

    bool CanHaveAttributes() override;

    wxString GetValue(int row, int col) override;
    void SetValue(int row, int col, const wxString &value) override;

    bool InsertRows(size_t pos, size_t numRows) override;
    bool AppendRows(size_t numRows) override;
    bool DeleteRows(size_t pos, size_t numRows) override;

protected:
    HaCsv *m_doc;
    wxVector<wxArrayString> m_cache;

    void CacheCell(int row, int col)
    {
        m_cache[row][col] = GetCellValue(row, col);
    }

    void CacheRow(int row)
    {
        m_cache[row].Empty();
        int cols = GetNumberCols();
        for (auto col = 0; col < cols; ++col) {
            m_cache[row].Add(GetCellValue(row, col));
        }
    }

    void CacheCol(int col)
    {
        for (auto i = 0; i < GetNumberRows(); ++i) {
            CacheCell(i, col);
        }
    }

    /**
     * @brief Refresh contents of a grid column and auto resize it to fit the contents.
     *
     * @param col the column
     */
    void RefreshAndAutoSizeGridColumn(int col)
    {
        auto grid = GetView();
        if (grid != nullptr) {
            grid->BeginBatch();
            grid->RefreshBlock(0, col, m_cache.size() - 1, col);
            grid->AutoSizeColumn(col);
            grid->EndBatch();
        }
    }

    wxString DocGetter(int row, int col) const
    {
        return m_doc->GetValueString(row, col);
    }

    void DocSetter(int row, int col, const wxString &value)
    {
        m_doc->SetValueString(row, col, value);
    }

    virtual wxString GetCommentString(int row) const;

    virtual bool InsertRow(size_t pos);
    virtual bool AppendRow();
    virtual bool DeleteRow(size_t pos);

    virtual void OnNewRow(size_t pos);

    virtual const wxString GetCellValue(int row, int col) const = 0;
    virtual void SetCellValue(int row, int col, const wxString &value) = 0;
};

#endif /* _HA_GUI_HA_TABLE_H_ */
