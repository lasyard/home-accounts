#ifndef _HA_PANEL_HA_DATA_TABLE_H_
#define _HA_PANEL_HA_DATA_TABLE_H_

#include "../HaTable.h"

class HaDataTable : public HaTable
{
    DECLARE_DYNAMIC_CLASS(HaDataTable)

public:
    static const int COLUMNS = 10;

    HaDataTable(CsvDoc *doc = nullptr);
    virtual ~HaDataTable();

    void Init() override;

    enum column_type GetItemFieldType(int col) const override
    {
        return col < COLUMNS ? m_colImpl[col].type : CT_IGNORE;
    }

private:
    struct ColImpl {
        enum column_type type;
        std::function<wxString(int)> get;
        std::function<void(int, const wxString &)> set;
    };

    static const int BALANCE_COL = 7;

    struct ColImpl m_colImpl[COLUMNS];

    money_t m_initial;

    void MapColImpl(struct ColImpl &colImpl, int col);

    const wxString GetItemCellValue(int row, int col) override;

    void SetItemCellValue(int row, int col, const wxString &value) override;

    struct data *GetData(int row) const;

    money_t GetInitial(int row) const;

    void CalcAndCacheBalanceFromRow(int row);
};

#endif /* _HA_PANEL_HA_DATA_TABLE_H_ */
