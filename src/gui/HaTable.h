#ifndef _HA_GUI_HA_TABLE_H_
#define _HA_GUI_HA_TABLE_H_

#include <wx/grid.h>
#include <wx/vector.h>

#include "CsvDoc.h"

#include "csv/column_type.h"

class HaTable : public wxGridTableBase
{
public:
    HaTable(std::initializer_list<const char *> colLabels, CsvDoc *doc);
    virtual ~HaTable();

    virtual void Init();

    int GetNumberRows() override;
    int GetNumberCols() override;

    wxString GetValue(int row, int col) override;
    wxString GetColLabelValue(int col) override;
    wxString GetRowLabelValue(int row) override;

    bool CanHaveAttributes() override;

    auto GetRowRecordFlag(int row) const
    {
        record_t *record = m_doc->GetRecord(row);
        if (record != nullptr) {
            return record->flag;
        }
        return (char)0;
    }

    virtual enum column_type GetColType(int col) const;

    auto *GetDoc()
    {
        return m_doc;
    }

    bool IsColReadOnly(int col) const
    {
        return m_colImpls[col].set == nullptr;
    }

    void SetValue(int row, int col, const wxString &value) override;

    bool InsertRows(size_t pos, size_t numRows) override;
    bool AppendRows(size_t numRows) override;
    bool DeleteRows(size_t pos, size_t numRows) override;

protected:
    struct ColImpl {
        enum column_type type;
        std::function<wxString(int)> get;
        std::function<void(int, const wxString &)> set;
    };

    CsvDoc *m_doc;
    int m_cols;
    wxArrayString m_colLabels;
    wxVector<wxArrayString> *m_cache;
    struct ColImpl *m_colImpls;

    void CacheCell(int row, int col)
    {
        (*m_cache)[row][col] = GetCellValue(row, col);
    }

    void CacheRow(int row)
    {
        (*m_cache)[row].Empty();
        int cols = GetNumberCols();
        for (auto col = 0; col < cols; ++col) {
            (*m_cache)[row].Add(GetCellValue(row, col));
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
            grid->RefreshBlock(0, col, m_cache->size() - 1, col);
            grid->AutoSizeColumn(col);
            grid->EndBatch();
        }
    }

    void MapColToCol(int dst, int col, bool ro = false);

    virtual wxString GetCommentString(int row);

    virtual bool InsertRow(size_t pos);
    virtual bool AppendRow();
    virtual bool DeleteRow(size_t pos);

    virtual void OnNewRow(size_t pos);

private:
    const wxString GetCellValue(int row, int col);
    void SetCellValue(int row, int col, const wxString &value);
};

#endif /* _HA_GUI_HA_TABLE_H_ */
