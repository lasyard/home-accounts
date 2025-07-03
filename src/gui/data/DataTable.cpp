#include "DataTable.h"

#include "DataDoc.h"
#include "DataGridCellAttrProvider.h"

#include "../Utils.h"

IMPLEMENT_DYNAMIC_CLASS(DataTable, HaImplTable)

DataTable::DataTable(DataDoc *doc)
    : HaImplTable(
          10,
          {
              TT("Time"),
              TT("Amount"),
              TT("Account"),
              TT("Description"),
              TT("Income"),
              TT("Outlay"),
              TT("Item"),
              TT("Balance"),
              TT("Memo"),
              TT("Category"),
          },
          doc
      )
{
    MapColImpl(m_colImpls[0], DATA_TIME_COL);
    MapColImpl(m_colImpls[1], DATA_AMOUNT_COL);
    MapColImpl(m_colImpls[2], DATA_ACCOUNT_COL);
    MapColImpl(m_colImpls[3], DATA_DESC_COL);
    m_colImpls[4] = {
        .type = m_doc->GetItemValueType(DATA_REAL_AMOUNT_COL),
        .get = [this](int row) -> wxString {
            return HaTable::GetItemCellMoneyValueBySign(row, DATA_REAL_AMOUNT_COL, true);
        },
        .set = [this](int row, const wxString &value) -> void {
            HaTable::SetItemCellValue(row, DATA_REAL_AMOUNT_COL, "-" + value);
            CacheCell(row, 5);
            UpdateDocAndCache(row);
        },
    };
    m_colImpls[OUTLAY_COL] = {
        .type = m_doc->GetItemValueType(DATA_REAL_AMOUNT_COL),
        .get = [this](int row) -> wxString {
            return HaTable::GetItemCellMoneyValueBySign(row, DATA_REAL_AMOUNT_COL, false);
        },
        .set = [this](int row, const wxString &value) -> void {
            HaTable::SetItemCellValue(row, DATA_REAL_AMOUNT_COL, value);
            CacheCell(row, 4);
            UpdateDocAndCache(row);
        },
    };
    MapColImpl(m_colImpls[6], DATA_REAL_DESC_COL);
    m_colImpls[BALANCE_COL] = {
        .type = CT_MONEY,
        .get = [this](int row) -> wxString { return m_doc->GetMoneyString(GetData(row)->balance); },
        .set = nullptr,
    };
    MapColImpl(m_colImpls[8], DATA_MEMO_COL);
    m_colImpls[9] = {
        .type = CT_IGNORE,
        .get = nullptr,
        .set = nullptr,
    };
}

DataTable::~DataTable()
{
}

void DataTable::Init()
{
    HaTable::Init();
    SetAttrProvider(new DataGridCellAttrProvider(this));
}

DataDoc *DataTable::GetDataDoc()
{
    return static_cast<DataDoc *>(m_doc);
}

struct data *DataTable::GetData(int row) const
{
    wxASSERT(m_index[row].GetType() == HaTableIndex::ITEM);
    return (struct data *)m_index[row].GetItem();
}

void DataTable::OnNewRow(size_t pos, void *item)
{
    HaTable::OnNewRow(pos, item);
    UpdateDocAndCache(pos);
}

void DataTable::UpdateDocAndCache(int row)
{
    GetDataDoc()->UpdateBalanceStat();
    for (auto i = row; i < GetNumberRows(); ++i) {
        if (GetRowType(i) != HaTableIndex::SEGMENT) {
            CacheCell(i, BALANCE_COL);
        }
    }
}
