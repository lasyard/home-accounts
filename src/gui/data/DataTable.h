#ifndef _HA_DATA_DATA_TABLE_H_
#define _HA_DATA_DATA_TABLE_H_

#include "../HaTableTemplate.h"

class DataDoc;

class DataTable : public HaTableTemplate<DataTable, DataDoc>
{
    DECLARE_DYNAMIC_CLASS(DataTable)

public:
    static constexpr int TIME_COL = 0;
    static constexpr int ACCOUNT_COL = 1;
    static constexpr int INCOME_COL = 2;
    static constexpr int OUTLAY_COL = 3;
    static constexpr int DESC_COL = 4;
    static constexpr int BALANCE_COL = 5;
    static constexpr int MEMO_COL = 6;
    static constexpr int CATEGORY_COL = 7;
    static constexpr int COLS = 8;

    DataTable(DataDoc *doc = nullptr);
    virtual ~DataTable();

    void Init() override;

protected:
    void OnNewRow(size_t pos) override;

private:
    void UpdateDocAndCache(int row);

    wxString IncomeGetter(int row, int col) const;
    void IncomeSetter(int row, int col, const wxString &value);
    wxString OutlayGetter(int row, int col) const;
    void OutlaySetter(int row, int col, const wxString &value);
    wxString BalanceGetter(int row, int col) const;
};

#endif /* _HA_DATA_DATA_TABLE_H_ */
