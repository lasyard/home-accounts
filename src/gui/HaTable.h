#ifndef _HA_GUI_HA_TABLE_H_
#define _HA_GUI_HA_TABLE_H_

#include <wx/grid.h>
#include <wx/vector.h>

#include "CsvDoc.h"
#include "HaGridCellAttrProvider.h"
#include "HaTableIndex.h"

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

    virtual ~HaTable();

    virtual void Init();

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

    HaTableIndex::Type GetRowType(int row) const
    {
        if ((size_t)row < m_index.size()) {
            return m_index[row].GetType();
        }
        return HaTableIndex::INVALID;
    }

    virtual enum column_type GetItemFieldType(int col) const
    {
        return m_doc->GetItemValueType(col);
    }

    auto *GetCsvDoc()
    {
        return m_doc;
    }

protected:
    CsvDoc *m_doc;
    wxArrayString m_colLabels;
    std::vector<HaTableIndex> m_index;
    wxVector<wxArrayString> *m_cache;

    void CacheCell(int row, int col);
    void CacheRow(int row);
    void CacheCol(int col);

    /**
     * @brief Refresh contents of a grid column and auto resize it to fit the contents.
     *
     * @param col the column
     */
    void RefreshAndAutoSizeGridColumn(int col);

    virtual const wxString GetCellValue(int row, int col);
    virtual const wxString GetItemCellValue(int row, int col);
    virtual const wxString GetSegmentCellValue(int row);

    virtual void SetCellValue(int row, int col, const wxString &value);
    virtual void SetItemCellValue(int row, int col, const wxString &value);
    virtual void SetSegmentCellValue(int row, const wxString &value);

    virtual bool InsertRow(size_t pos);
    virtual bool AppendRow();
    virtual bool DeleteRow(size_t pos);

    virtual void OnNewRow(size_t pos, void *item);
};

#endif /* _HA_GUI_HA_TABLE_H_ */
