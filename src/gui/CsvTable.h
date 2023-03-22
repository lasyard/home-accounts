#ifndef _GUI_CSV_TABLE_H_
#define _GUI_CSV_TABLE_H_

#include "CachedTable.h"
#include "data/CsvVecDao.h"

class CsvTableBase : public CachedTable
{
public:
    CsvTableBase(size_t cols, const wxString *const colLabels) : CachedTable(cols, colLabels)
    {
    }

    virtual ~CsvTableBase()
    {
    }

    virtual ColumnType GetColumnType(int col) const = 0;
};

/**
 * @brief Table to reader csv.
 *
 * @tparam I the element type of row
 */
template <typename I> class CsvTable : public CsvTableBase
{
    typedef CsvRowTraits<I> Traits;

public:
    CsvTable(size_t cols, const wxString *const colLabels, CsvVecDao<I> *dao)
        : CsvTableBase(cols, colLabels), m_dao(dao)
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

private:
    CsvVecDao<I> *m_dao;

    wxString GetCellValue(int row, int col) override
    {
        return m_dao->getString(row, col);
    }

    void SetCellValue(int row, int col, const std::string &value) override
    {
        m_dao->setString(row, col, value);
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
