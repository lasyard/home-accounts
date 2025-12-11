#ifndef _HA_GUI_HA_TABLE_H_
#define _HA_GUI_HA_TABLE_H_

#include <wx/grid.h>
#include <wx/vector.h>

#include "CsvDoc.h"

#include "csv/column_type.h"

class HaTable : public wxGridTableBase
{
public:
    HaTable(std::initializer_list<const char *> colLabels, CsvDoc *doc = nullptr)
        : wxGridTableBase()
        , m_doc(doc)
        , m_colLabels(colLabels)
        , m_cache(nullptr)
    {
        m_cols = m_colLabels.size();
        m_colImpls = new struct ColImpl[m_cols];
    }

    virtual ~HaTable()
    {
        if (m_cache != nullptr) {
            delete m_cache;
        }
        if (m_doc != nullptr) {
            delete m_doc;
        }
        delete[] m_colImpls;
    }

    virtual void Init();

    int GetNumberRows() override
    {
        return m_cache != nullptr ? m_cache->size() : 0;
    }

    int GetNumberCols() override
    {
        return m_colLabels.size();
    }

    wxString GetValue(int row, int col) override
    {
        return (*m_cache)[row][col];
    }

    wxString GetColLabelValue(int col) override
    {
        return m_colLabels[col];
    }

    wxString GetRowLabelValue(int row) override
    {
        if (row < GetNumberRows()) {
            return wxString::Format("%d", row);
        }
        return wxEmptyString;
    }

    bool CanHaveAttributes() override
    {
        return true;
    }

    virtual enum column_type GetColType(int col) const
    {
        return col < m_cols ? m_colImpls[col].type : CT_IGNORE;
    }

    auto *GetDoc()
    {
        return m_doc;
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

    void MapColImplToDoc(struct ColImpl &colImpl, int col)
    {
        colImpl.type = m_doc->GetColType(col);
        colImpl.get = [this, col](int row) -> wxString { return m_doc->GetValueString(row, col); };
        colImpl.set = [this, col](int row, const wxString &value) -> void {
            m_doc->SetValueString(row, col, value);
        };
    }

    void MapAllColsToDoc()
    {
        wxASSERT(m_cols == m_doc->GetColCount());
        for (int i = 0; i < m_cols; ++i) {
            MapColImplToDoc(m_colImpls[i], i);
        }
    }

    virtual bool InsertRow(size_t pos);
    virtual bool AppendRow();
    virtual bool DeleteRow(size_t pos);

private:
    const wxString GetCellValue(int row, int col)
    {
        if (col < m_cols) {
            if (m_colImpls[col].get != nullptr) {
                return m_colImpls[col].get(row);
            }
            return _("not implemented");
        }
        return wxEmptyString;
    }

    void SetCellValue(int row, int col, const wxString &value)
    {
        if (col < m_cols && m_colImpls[col].set != nullptr) {
            m_colImpls[col].set(row, value);
        }
    }
};

#endif /* _HA_GUI_HA_TABLE_H_ */
