#ifndef _HA_CACHED_TABLE_H_
#define _HA_CACHED_TABLE_H_

#include <wx/grid.h>
#include <wx/vector.h>

class CachedTable : public wxGridTableBase
{
public:
    CachedTable(const wxArrayString &colLabels) : wxGridTableBase(), m_colLabels(colLabels), m_cache(nullptr)
    {
    }

    virtual ~CachedTable()
    {
        if (m_cache != nullptr) {
            delete m_cache;
        }
    }

    void InitCache(int rows)
    {
        m_cache = new wxVector<wxArrayString>(rows);
        for (auto i = 0; i < GetNumberRows(); ++i) {
            CacheRow(i);
        }
    }

    int GetNumberRows() override
    {
        return m_cache->size();
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

    void SetValue(int row, int col, const wxString &value) override;

    bool InsertRows(size_t pos, size_t numRows) override;

    bool CanHaveAttributes() override
    {
        return true;
    }

protected:
    wxArrayString m_colLabels;
    wxVector<wxArrayString> *m_cache;

    void CacheRow(int row);
    void CacheCell(int row, int col);

    virtual wxString GetCellValue(int row, int col) = 0;

    virtual void SetCellValue(int row, int col, const std::string &value) = 0;

    virtual bool InsertRow(int pos) = 0;
};

#endif /* _HA_CACHED_TABLE_H_ */
