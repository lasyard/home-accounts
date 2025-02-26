#include "HaDataTable.h"

#include "HaDataGridCellAttrProvider.h"

#include "../Utils.h"

#include "data/data.h"

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
              TT("balance"),
              TT("memo"),
              TT("category"),
          }
      )
    , m_initial(0L)
{
    MapColImpl(m_colImpl[0], DATA_TIME_COL);
    MapColImpl(m_colImpl[1], DATA_AMOUNT_COL);
    MapColImpl(m_colImpl[2], DATA_ACCOUNT_COL);
    MapColImpl(m_colImpl[3], DATA_DESC_COL);
    m_colImpl[4] = {
        .type = m_doc->GetItemValueType(DATA_REAL_AMOUNT_COL),
        .get = [this](int row) -> wxString {
            return HaTable::GetItemCellMoneyValueBySign(row, DATA_REAL_AMOUNT_COL, true);
        },
        .set = [this](int row, const wxString &value) -> void {
            HaTable::SetItemCellValue(row, DATA_REAL_AMOUNT_COL, "-" + value);
            CacheCell(row, 5);
            CalcAndCacheBalanceFromRow(row);
        },
    };
    m_colImpl[5] = {
        .type = m_doc->GetItemValueType(DATA_REAL_AMOUNT_COL),
        .get = [this](int row) -> wxString {
            return HaTable::GetItemCellMoneyValueBySign(row, DATA_REAL_AMOUNT_COL, false);
        },
        .set = [this](int row, const wxString &value) -> void {
            HaTable::SetItemCellValue(row, DATA_REAL_AMOUNT_COL, value);
            CacheCell(row, 4);
            CalcAndCacheBalanceFromRow(row);
        },
    };
    MapColImpl(m_colImpl[6], DATA_REAL_DESC_COL);
    m_colImpl[BALANCE_COL] = {
        .type = CT_MONEY,
        .get = [this](int row) -> wxString { return m_doc->GetMoneyString(GetData(row)->balance); },
        .set = nullptr,
    };
    MapColImpl(m_colImpl[8], DATA_MEMO_COL);
    m_colImpl[9] = {
        .type = CT_IGNORE,
        .get = nullptr,
        .set = nullptr,
    };
}

HaDataTable::~HaDataTable()
{
}

void HaDataTable::Init()
{
    HaTable::Init();
    SetAttrProvider(new HaDataGridCellAttrProvider(this));
}

void HaDataTable::MapColImpl(struct ColImpl &colImpl, int col)
{
    colImpl.type = m_doc->GetItemValueType(col);
    colImpl.get = [this, col](int row) -> wxString { return HaTable::GetItemCellValue(row, col); };
    colImpl.set = [this, col](int row, const wxString &value) -> void {
        return HaTable::SetItemCellValue(row, col, value);
    };
}

const wxString HaDataTable::GetItemCellValue(int row, int col)
{
    if (col < COLUMNS) {
        if (m_colImpl[col].get != nullptr) {
            return m_colImpl[col].get(row);
        }
        return _("not implemented");
    }
    return wxEmptyString;
}

void HaDataTable::SetItemCellValue(int row, int col, const wxString &value)
{
    if (col < COLUMNS && m_colImpl[col].set != nullptr) {
        m_colImpl[col].set(row, value);
    }
}

struct data *HaDataTable::GetData(int row) const
{
    wxASSERT(m_index[row].GetType() == HaTableIndex::ITEM);
    return (struct data *)m_index[row].GetItem();
}

money_t HaDataTable::GetInitial(int row) const
{
    wxASSERT(row > 1);
    while (--row > 0 && GetRowType(row) == HaTableIndex::SEGMENT)
        ;
    return row == 0 ? m_initial : GetData(row)->balance;
}

void HaDataTable::CalcAndCacheBalanceFromRow(int row)
{
    calc_balance(m_index[row].GetSegment(), GetData(row), GetInitial(row));
    for (auto i = row; i < GetNumberRows(); ++i) {
        if (GetRowType(i) != HaTableIndex::SEGMENT) {
            CacheCell(i, BALANCE_COL);
        }
    }
}
