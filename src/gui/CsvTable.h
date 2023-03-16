#ifndef _GUI_CSV_TABLE_H_
#define _GUI_CSV_TABLE_H_

#include "CachedTable.h"
#include "CellAttrs.h"
#include "data/CsvIdVecDao.h"

/**
 * @brief Table to reader csv.
 *
 * @tparam I the element type of row
 */
template <typename I> class CsvTable : public CachedTable
{
    typedef CsvRowTraits<I> Traits;

public:
    CsvTable(size_t cols, const wxString *const colLabels, CsvVecDao<I> *dao) : CachedTable(cols, colLabels), m_dao(dao)
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

    wxGridCellAttr *
    GetAttr([[maybe_unused]] int row, int col, [[maybe_unused]] wxGridCellAttr::wxAttrKind kind) override
    {
        auto type = Traits::types[col];
        switch (type) {
        case INT32:
        case INT64:
        case MONEY:
            return CellAttrs::ins().GetNumber();
        case DATE:
        case TIME:
            return CellAttrs::ins().GetTime();
        default:
            break;
        }
        return CellAttrs::ins().GetDefault();
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
