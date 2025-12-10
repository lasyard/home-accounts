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
    }

    virtual ~HaTable()
    {
        if (m_cache != nullptr) {
            delete m_cache;
        }
        if (m_doc != nullptr) {
            delete m_doc;
        }
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

    virtual enum column_type GetItemFieldType(int col) const
    {
        return m_doc->GetColType(col);
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
    CsvDoc *m_doc;
    wxArrayString m_colLabels;
    wxVector<wxArrayString> *m_cache;

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

    virtual const wxString GetCellValue(int row, int col)
    {
        return m_doc->GetRecordValueString(row, col);
    }

    virtual void SetCellValue(int row, int col, const wxString &value)
    {
        m_doc->SetRecordValueString(row, col, value);
    }

    virtual bool InsertRow(size_t pos);
    virtual bool AppendRow();
    virtual bool DeleteRow(size_t pos);
};

#endif /* _HA_GUI_HA_TABLE_H_ */
