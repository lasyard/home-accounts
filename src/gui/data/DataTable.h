#ifndef _HA_DATA_DATA_TABLE_H_
#define _HA_DATA_DATA_TABLE_H_

#include "../HaTable.h"

class DataDoc;

class DataTable : public HaTable
{
    DECLARE_DYNAMIC_CLASS(DataTable)

public:
    static const int TIME_COL = 0;
    static const int ACCOUNT_COL = 1;
    static const int AMOUNT_COL = 2;
    static const int DESC_COL = 3;
    static const int INCOME_COL = 4;
    static const int OUTLAY_COL = 5;
    static const int REAL_DESC_COL = 6;
    static const int BALANCE_COL = 7;
    static const int MEMO_COL = 8;
    static const int CATEGORY_COL = 9;

    DataTable(DataDoc *doc = nullptr);
    virtual ~DataTable();

    void Init() override;

    DataDoc *GetDataDoc();

protected:
    void OnNewRow(size_t pos) override;

private:
    void UpdateDocAndCache(int row);
};

#endif /* _HA_DATA_DATA_TABLE_H_ */
