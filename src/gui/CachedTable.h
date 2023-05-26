#ifndef _GUI_CACHED_TABLE_H_
#define _GUI_CACHED_TABLE_H_

#include <wx/grid.h>
#include <wx/vector.h>

#include "csv/ColumnType.h"

class DaoBase;

class CachedTable : public wxGridTableBase
{
public:
    CachedTable(size_t cols, const wxString colLabels[]);
    virtual ~CachedTable();

    void CacheRow(int row);
    void CacheCol(int col);

    int GetNumberRows() override;
    int GetNumberCols() override;

    wxString GetValue(int row, int col) override;
    wxString GetColLabelValue(int col) override;

    void SetValue(int row, int col, const wxString &value) override;

    bool InsertRows(size_t pos, size_t numRows) override;
    bool AppendRows(size_t numRows) override;
    bool DeleteRows(size_t pos, size_t numRows) override;

    bool CanHaveAttributes() override;

    const std::string &GetDaoName() const;

    virtual DaoBase *GetDao() = 0;
    virtual const DaoBase *GetDao() const = 0;
    virtual ColumnType GetColumnType(int col) const = 0;

    virtual CachedTable *Clone() const = 0;

protected:
    wxArrayString m_colLabels;
    wxVector<wxArrayString> *m_cache;

    void CreateCache(int rows);

    void CacheCell(int row, int col);

    /**
     * @brief Refresh contents of a grid column and auto resize it to fit the contents.
     *
     * @param col the column
     */
    void RefreshAndAutoSizeGridColumn(int col);

    virtual wxString GetCellValue(int row, int col) = 0;

    virtual void SetCellValue(int row, int col, const wxString &value) = 0;

    virtual bool InsertRow(size_t pos) = 0;
    virtual bool AppendRow() = 0;
    virtual bool DeleteRow(size_t pos) = 0;
};

#endif /* _GUI_CACHED_TABLE_H_ */
