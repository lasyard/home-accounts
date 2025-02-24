#include "HaDataTable.h"

#include "HaDataGridCellAttrProvider.h"

#include "../Utils.h"

IMPLEMENT_DYNAMIC_CLASS(HaDataTable, HaTable)

HaDataTable::HaDataTable(CsvDoc *doc)
    : HaTable(
          doc,
          {
              TT("time"),
              TT("amount"),
              TT("account"),
              TT("desc"),
              TT("income"),
              TT("outlay"),
              TT("item"),
              TT("memo"),
              TT("category"),
          }
      )
{
}

HaDataTable::~HaDataTable()
{
}

void HaDataTable::Init()
{
    HaTable::Init();
    SetAttrProvider(new HaDataGridCellAttrProvider(this));
}

const wxString HaDataTable::GetItemCellValue(int row, int col)
{
    switch (col) {
    case TIME_COL:
        return HaTable::GetItemCellValue(row, DATA_TIME_COL);
    case AMOUNT_COL:
        return HaTable::GetItemCellValue(row, DATA_AMOUNT_COL);
    case ACCOUNT_COL:
        return HaTable::GetItemCellValue(row, DATA_ACCOUNT_COL);
    case DESC_COL:
        return HaTable::GetItemCellValue(row, DATA_DESC_COL);
    case INCOME_COL:
        return HaTable::GetItemCellMoneyValueBySign(row, DATA_REAL_AMOUNT_COL, true);
    case OUTLAY_COL:
        return HaTable::GetItemCellMoneyValueBySign(row, DATA_REAL_AMOUNT_COL, false);
    case ITEM_COL:
        return HaTable::GetItemCellValue(row, DATA_REAL_DESC_COL);
    case MEMO_COL:
        return HaTable::GetItemCellValue(row, DATA_MEMO_COL);
    case CATEGORY_COL:
    default:
        break;
    }
    return wxEmptyString;
}

void HaDataTable::SetItemCellValue(int row, int col, const wxString &value)
{
    switch (col) {
    case TIME_COL:
        HaTable::SetItemCellValue(row, DATA_TIME_COL, value);
        break;
    case AMOUNT_COL:
        HaTable::SetItemCellValue(row, DATA_AMOUNT_COL, value);
        break;
    case ACCOUNT_COL:
        HaTable::SetItemCellValue(row, DATA_ACCOUNT_COL, value);
        break;
    case DESC_COL:
        HaTable::SetItemCellValue(row, DATA_DESC_COL, value);
        break;
    case INCOME_COL:
        HaTable::SetItemCellValue(row, DATA_REAL_AMOUNT_COL, "-" + value);
        CacheCell(row, OUTLAY_COL);
        break;
    case OUTLAY_COL:
        HaTable::SetItemCellValue(row, DATA_REAL_AMOUNT_COL, value);
        CacheCell(row, INCOME_COL);
        break;
    case ITEM_COL:
        HaTable::SetItemCellValue(row, DATA_REAL_DESC_COL, value);
        break;
    case MEMO_COL:
        HaTable::SetItemCellValue(row, DATA_MEMO_COL, value);
        break;
    case CATEGORY_COL:
    default:
        break;
    }
}
