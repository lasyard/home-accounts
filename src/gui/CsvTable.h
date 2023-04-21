#ifndef _GUI_CSV_TABLE_H_
#define _GUI_CSV_TABLE_H_

#include <functional>

#include "CachedTable.h"
#include "data/CsvVecDao.h"

/**
 * @brief Table to reader csv.
 *
 * @tparam I the element type of row
 */
template <typename I> class CsvTable : public CachedTable
{
    typedef CsvRowTraits<I> Traits;

public:
    CsvTable(const wxString &name, size_t cols, const wxString *const colLabels, CsvVecDao<I> *dao)
        : CachedTable(name, cols, colLabels), m_dao(dao)
    {
        InitCache(dao->getNumberRows());
    }

    virtual ~CsvTable()
    {
    }

    wxString GetRowLabelValue(int row) override
    {
        return m_dao->getRowLabel(row);
    }

    ColumnType GetColumnType(int col) const override
    {
        return Traits::types[col];
    }

    DaoBase *GetDao() override
    {
        return m_dao;
    }

    const DaoBase *GetDao() const override
    {
        return m_dao;
    }

private:
    CsvVecDao<I> *m_dao;

    wxString GetCellValue(int row, int col) override
    {
        return m_dao->getString(row, col);
    }

    void SetCellValue(int row, int col, const wxString &value) override
    {
        m_dao->setString(row, col, value.ToStdString());
    }

    bool InsertRow(size_t pos) override
    {
        return m_dao->insert(pos);
    }

    bool AppendRow() override
    {
        return m_dao->append();
    }

    bool DeleteRow(size_t pos) override
    {
        return m_dao->remove(pos);
    }
};

#endif /* _GUI_CSV_TABLE_H_ */
