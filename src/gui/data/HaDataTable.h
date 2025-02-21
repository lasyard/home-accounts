#ifndef _HA_PANEL_HA_DATA_TABLE_H_
#define _HA_PANEL_HA_DATA_TABLE_H_

#include "../HaTable.h"

#include "data/data.h"

class HaDataTable : public HaTable
{
    DECLARE_DYNAMIC_CLASS(HaDataTable)

public:
    HaDataTable(CsvDoc *doc = nullptr);
    virtual ~HaDataTable();

    void Init() override;

    enum column_type GetItemFieldType(int col) const override
    {
        switch (col) {
        case TIME_COL:
            return m_doc->GetItemValueType(DATA_TIME_COL);
        case AMOUNT_COL:
            return m_doc->GetItemValueType(DATA_AMOUNT_COL);
        case ACCOUNT_COL:
            return m_doc->GetItemValueType(DATA_ACCOUNT_COL);
        case DESC_COL:
            return m_doc->GetItemValueType(DATA_DESC_COL);
        case INCOME_COL:
        case OUTLAY_COL:
            return m_doc->GetItemValueType(DATA_REAL_AMOUNT_COL);
        case ITEM_COL:
            return m_doc->GetItemValueType(DATA_REAL_DESC_COL);
        case MEMO_COL:
            return m_doc->GetItemValueType(DATA_MEMO_COL);
        case CATEGORY_COL:
            return CT_INT32;
        default:
            break;
        }
        return CT_IGNORE;
    }

private:
    static const int TIME_COL = 0;
    static const int AMOUNT_COL = 1;
    static const int ACCOUNT_COL = 2;
    static const int DESC_COL = 3;
    static const int INCOME_COL = 4;
    static const int OUTLAY_COL = 5;
    static const int ITEM_COL = 6;
    static const int MEMO_COL = 7;
    static const int CATEGORY_COL = 8;

    const wxString GetItemCellValue(int row, int col) override;

    void SetItemCellValue(int row, int col, const wxString &value) override;
};

#endif /* _HA_PANEL_HA_DATA_TABLE_H_ */
