#ifndef _HA_CSV_TABLE_H_
#define _HA_CSV_TABLE_H_

#include "CachedTable.h"
#include "data/CsvVecDao.h"

template <typename I> class CsvTable : public CachedTable
{
public:
    CsvTable(const wxArrayString &colLabels, CsvVecDao<I> *dao) : CachedTable(colLabels), m_dao(dao)
    {
        InitCache(dao->getNumberRows());
    }

    virtual ~CsvTable()
    {
    }

    wxString GetRowLabelValue(int row) override
    {
        return wxString::Format("%3d", row);
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

    bool InsertRow([[maybe_unused]] int pos) override
    {
        return false;
    }
};

#endif /* _HA_CSV_TABLE_H_ */
