#ifndef _HA_GUI_HA_TABLE_H_
#define _HA_GUI_HA_TABLE_H_

#include <wx/grid.h>
#include <wx/vector.h>

#include "CsvDoc.h"

#include "csv/column_type.h"

class HaTable : public wxGridTableBase
{
public:
    HaTable(std::initializer_list<const char *> colLabels, CsvDoc *doc)
        : wxGridTableBase()
        , m_doc(doc)
        , m_colLabels(colLabels)
        , m_cache(nullptr)
    {
        m_cols = m_colLabels.size();
        m_colImpls = new struct ColImpl[m_cols];
    }

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

    void MapColToCol(int dst, int col, bool ro = false)
    {
        auto &colImpl = m_colImpls[dst];
        colImpl.type = m_doc->GetColType(col);
        colImpl.get = [this, col](int row) -> wxString { return m_doc->GetValueString(row, col); };
        if (!ro) {
            colImpl.set = [this, col](int row, const wxString &value) -> void {
                m_doc->SetValueString(row, col, value);
            };
        }
    }

    virtual wxString GetCommentString(int row);

    virtual bool InsertRow(size_t pos);
    virtual bool AppendRow();
    virtual bool DeleteRow(size_t pos);

private:
    const wxString GetCellValue(int row, int col)
    {
        auto flag = GetRowRecordFlag(row);
        if (flag == RECORD_FLAG_COMMENT) {
            if (col == 0) {
                return GetCommentString(row);
            }
        } else if (col < m_cols) {
            if (m_colImpls[col].get != nullptr) {
                return m_colImpls[col].get(row);
            }
            return _("not implemented");
        }
        return wxEmptyString;
    }

    void SetCellValue(int row, int col, const wxString &value)
    {
        wxASSERT(GetRowRecordFlag(row) != RECORD_FLAG_COMMENT);
        if (col < m_cols && m_colImpls[col].set != nullptr) {
            m_colImpls[col].set(row, value);
        }
    }
};

#endif /* _HA_GUI_HA_TABLE_H_ */
