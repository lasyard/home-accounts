#include "DataTable.h"

#include "DataDoc.h"
#include "DataGridCellAttrProvider.h"

IMPLEMENT_DYNAMIC_CLASS(DataTable, HaTable)

DataTable::DataTable(DataDoc *doc) : HaTable(doc)
{
}

DataTable::~DataTable()
{
}

void DataTable::Prepare()
{
    m_colImpls.resize(COLS);
    SetColImplDoc(_("Time"), TIME_COL, HaData::TIME_COL);
    SetColImplDoc(_("Account"), ACCOUNT_COL, HaData::ACCOUNT_COL);
    SetColImpl(_("Income"), INCOME_COL, CT_MONEY, &DataTable::IncomeGetter, &DataTable::IncomeSetter);
    SetColImpl(_("Outlay"), OUTLAY_COL, CT_MONEY, &DataTable::OutlayGetter, &DataTable::OutlaySetter);
    SetColImplDoc(_("Description"), DESC_COL, HaData::DESC_COL);
    SetColImpl(_("Balance"), BALANCE_COL, CT_MONEY, &DataTable::BalanceGetter);
    SetColImplDoc(_("Memo"), MEMO_COL, HaData::MEMO_COL);
    SetColImpl(_("Category"), CATEGORY_COL, CT_IGNORE);
    m_cache.resize(m_doc->GetRowCount());
    SetAttrProvider(new DataGridCellAttrProvider(this, static_cast<DataDoc *>(m_doc)->GetAccountNames()));
}

void DataTable::OnNewRow(size_t pos)
{
    HaTable::OnNewRow(pos);
    UpdateDocAndCache(pos);
}

const wxString DataTable::IncomeGetter(const HaTable *table, int row, [[maybe_unused]] int col)
{
    return static_cast<const DataDoc *>(table->GetDoc())->GetIncomeString(row);
}

void DataTable::IncomeSetter(HaTable *table, int row, [[maybe_unused]] int col, const wxString &value)
{
    static_cast<DataDoc *>(table->GetDoc())->SetValueString(row, HaData::AMOUNT_COL, "-" + value);
    auto *dt = static_cast<DataTable *>(table);
    dt->CacheCell(row, OUTLAY_COL);
    dt->UpdateDocAndCache(row);
}

const wxString DataTable::OutlayGetter(const HaTable *table, int row, [[maybe_unused]] int col)
{
    return static_cast<const DataDoc *>(table->GetDoc())->GetOutlayString(row);
}

void DataTable::OutlaySetter(HaTable *table, int row, [[maybe_unused]] int col, const wxString &value)
{
    static_cast<DataDoc *>(table->GetDoc())->SetValueString(row, HaData::AMOUNT_COL, value);
    auto *dt = static_cast<DataTable *>(table);
    dt->CacheCell(row, INCOME_COL);
    dt->UpdateDocAndCache(row);
}

const wxString DataTable::BalanceGetter(const HaTable *table, int row, [[maybe_unused]] int col)
{
    return static_cast<const DataDoc *>(table->GetDoc())->GetBalanceString(row);
}

void DataTable::UpdateDocAndCache(int row)
{
    static_cast<DataDoc *>(m_doc)->UpdateBalanceStat();
    for (auto i = row; i < GetNumberRows(); ++i) {
        CacheCell(i, BALANCE_COL);
    }
}
