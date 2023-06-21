#ifndef _DATA_DATA_TABLE_H_
#define _DATA_DATA_TABLE_H_

#include "../CachedTable.h"

#include "data/DataDao.h"

class DataTable : public CachedTable
{
public:
    static const wxString COL_LABELS[];

    static const int TIME_COL = 0;
    static const int INCOME_COL = 1;
    static const int OUTLAY_COL = 2;
    static const int ACCOUNT_COL = 3;
    static const int DESC_COL = 4;
    static const int BALANCE_COL = 5;
    static const int VALID_COL = 6;

    static const size_t COL_NUM = 7;

    DataTable(DataDao *dataDao);
    virtual ~DataTable();

    auto GetRowType(int row) const
    {
        if (row < m_dataDao->getNumberRows()) {
            return m_dataDao->getRowType(row);
        }
        return DataDao::IndexType::OTHER;
    }

    wxString GetRowLabelValue(int row) override;

    DataDao *GetDao() override;
    const DataDao *GetDao() const override;
    ColumnType GetColumnType(int col) const override;

    void UpdateChoicesFromJoints();

    DataTable *Clone() const override
    {
        return new DataTable(m_dataDao);
    }

private:
    DataDao *m_dataDao;

    wxString GetCellValue(int row, int col) override;

    void SetCellValue(int row, int col, const wxString &value) override;

    bool InsertRow(size_t pos) override;
    bool AppendRow() override;

    bool DeleteRow([[maybe_unused]] size_t pos) override
    {
        // Do nothing, deletion is done in `DataGrid`.
        return true;
    }

    void CacheBalances(int row);
    void CacheTotal();
};

#endif /* _DATA_DATA_TABLE_H_ */
