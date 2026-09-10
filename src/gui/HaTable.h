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

    virtual void Prepare();

    void Init();

    int GetNumberRows() override;
    int GetNumberCols() override;

    virtual enum column_type GetColType(int col) const;
    virtual bool IsColReadOnly(int col) const;

    wxString GetColLabelValue(int col) override;
    wxString GetRowLabelValue(int row) override;

    virtual record_t *GetRowRecord(int row) const;

    auto GetRowRecordFlag(int row) const
    {
        record_t *record = GetRowRecord(row);
        return (record != nullptr) ? record->flag : RECORD_FLAG_INVALID;
    }

    bool CanHaveAttributes() override;

    const HaCsv *GetDoc() const
    {
        return m_doc;
    }

    HaCsv *GetDoc()
    {
        return m_doc;
    }

    wxString GetValue(int row, int col) override;
    void SetValue(int row, int col, const wxString &value) override;

    bool InsertRows(size_t pos, size_t numRows) override;
    bool AppendRows(size_t numRows) override;
    bool DeleteRows(size_t pos, size_t numRows) override;

protected:
    struct CellImpl {
        wxString label;
        enum column_type type;
        const wxString (*get)(const HaTable *table, int row, int col);
        void (*set)(HaTable *table, int row, int col, const wxString &value);
        int pos; // save corresponding row/col in the doc
    };

    HaCsv *m_doc;
    wxVector<wxArrayString> m_cache;
    std::vector<struct CellImpl> m_headerImpls;
    std::vector<struct CellImpl> m_colImpls;

    static const wxString DocGetter(const HaTable *table, int row, int col);
    static void DocSetter(HaTable *table, int row, int col, const wxString &value);

    void SetImpl(
        const wxString &label,
        CellImpl &impl,
        enum column_type type,
        const wxString (*get)(const HaTable *table, int row, int col) = nullptr,
        void (*set)(HaTable *table, int row, int col, const wxString &value) = nullptr,
        int pos = -1
    )
    {
        impl.label = label;
        impl.type = type;
        impl.get = get;
        impl.set = set;
        impl.pos = pos;
    }

    void SetColImpl(
        const wxString &label,
        int dst,
        enum column_type type,
        const wxString (*get)(const HaTable *table, int row, int col) = nullptr,
        void (*set)(HaTable *table, int row, int col, const wxString &value) = nullptr,
        int pos = -1
    )
    {
        SetImpl(label, m_colImpls[dst], type, get, set, pos);
    }

    void SetColImplDoc(const wxString &label, int dst, int col, bool ro = false)
    {
        SetColImpl(label, dst, m_doc->GetColType(col), &HaTable::DocGetter, !ro ? &HaTable::DocSetter : nullptr, col);
    }

    void SetHeaderImpl(
        const wxString &label,
        int row,
        enum column_type type,
        const wxString (*get)(const HaTable *table, int row, int col) = nullptr,
        void (*set)(HaTable *table, int row, int col, const wxString &value) = nullptr
    )
    {
        SetImpl(label, m_headerImpls[row], type, get, set, row);
    }

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

    virtual wxString GetHashString(int row) const;

    virtual bool InsertRow(size_t pos);
    virtual bool AppendRow();
    virtual bool DeleteRow(size_t pos);

    virtual void OnNewRow(size_t pos);

    virtual const wxString GetCellValue(int row, int col) const;
    virtual void SetCellValue(int row, int col, const wxString &value);
};

#endif /* _HA_GUI_HA_TABLE_H_ */
