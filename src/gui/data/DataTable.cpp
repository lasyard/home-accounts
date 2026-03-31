#include "DataTable.h"

#include "DataDoc.h"
#include "DataGridCellAttrProvider.h"

#include "../Utils.h"

IMPLEMENT_DYNAMIC_CLASS(DataTable, HaTable)

DataTable::DataTable(DataDoc *doc) : HaTableTemplate<DataTable, DataDoc>(doc)
{
}

DataTable::~DataTable()
{
}

void DataTable::Init()
{
    m_colImpls.resize(COLS);
    SetColImplDoc(_("Time"), TIME_COL, DataDoc::TIME_COL);
    SetColImplDoc(_("Amount"), AMOUNT_COL, DataDoc::AMOUNT_COL);
    SetColImplDoc(_("Account"), ACCOUNT_COL, DataDoc::ACCOUNT_COL);
    SetColImplDoc(_("Description"), DESC_COL, DataDoc::DESC_COL);
    SetColImpl(_("Income"), INCOME_COL, CT_MONEY, &DataTable::IncomeGetter, &DataTable::IncomeSetter);
    SetColImpl(_("Outlay"), OUTLAY_COL, CT_MONEY, &DataTable::OutlayGetter, &DataTable::OutlaySetter);
    SetColImplDoc(_("Item"), REAL_DESC_COL, DataDoc::REAL_DESC_COL);
    SetColImpl(_("Balance"), BALANCE_COL, CT_MONEY, &DataTable::BalanceGetter);
    SetColImplDoc(_("Memo"), MEMO_COL, DataDoc::MEMO_COL);
    SetColImpl(_("Category"), CATEGORY_COL, CT_IGNORE);
    m_cache.resize(m_doc->GetRowCount());
    SetAttrProvider(new DataGridCellAttrProvider(this, GetDoc()->GetAccountNames()));
    HaTable::Init();
}

void DataTable::OnNewRow(size_t pos)
{
    HaTable::OnNewRow(pos);
    UpdateDocAndCache(pos);
}

void DataTable::UpdateDocAndCache(int row)
{
    GetDoc()->UpdateBalanceStat();
    for (auto i = row; i < GetNumberRows(); ++i) {
        CacheCell(i, BALANCE_COL);
    }
}

wxString DataTable::IncomeGetter(int row, [[maybe_unused]] int col) const
{
    return GetDoc()->GetIncomeString(row);
}

void DataTable::IncomeSetter(int row, [[maybe_unused]] int col, const wxString &value)
{
    m_doc->SetValueString(row, DataDoc::REAL_AMOUNT_COL, "-" + value);
    CacheCell(row, OUTLAY_COL);
    UpdateDocAndCache(row);
}

wxString DataTable::OutlayGetter(int row, [[maybe_unused]] int col) const
{
    return GetDoc()->GetOutlayString(row);
}

void DataTable::OutlaySetter(int row, [[maybe_unused]] int col, const wxString &value)
{
    m_doc->SetValueString(row, DataDoc::REAL_AMOUNT_COL, value);
    CacheCell(row, INCOME_COL);
    UpdateDocAndCache(row);
}

wxString DataTable::BalanceGetter(int row, [[maybe_unused]] int col) const
{
    return GetDoc()->GetBalanceString(row);
}
