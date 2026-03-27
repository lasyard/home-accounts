#include "DataTable.h"

#include "DataDoc.h"
#include "DataGridCellAttrProvider.h"

#include "../Utils.h"

IMPLEMENT_DYNAMIC_CLASS(DataTable, HaTable)

DataTable::DataTable(DataDoc *doc) : HaTableTemplate<DataDoc>(doc)
{
}

DataTable::~DataTable()
{
}

void DataTable::Init()
{
    m_colImpls.resize(COLS);
    SetColImpl(_("Time"), TIME_COL, DataDoc::TIME_COL);
    SetColImpl(_("Amount"), AMOUNT_COL, DataDoc::AMOUNT_COL);
    SetColImpl(_("Account"), ACCOUNT_COL, DataDoc::ACCOUNT_COL);
    SetColImpl(_("Description"), DESC_COL, DataDoc::DESC_COL);
    auto *doc = GetDoc();
    m_colImpls[INCOME_COL] = {
        .label = _("Income"),
        .type = CT_MONEY,
        .get = [doc](int row) -> wxString { return doc->GetIncomeString(row); },
        .set = [this](int row, const wxString &value) -> void {
            m_doc->SetValueString(row, DataDoc::REAL_AMOUNT_COL, "-" + value);
            CacheCell(row, OUTLAY_COL);
            UpdateDocAndCache(row);
        },
    };
    m_colImpls[OUTLAY_COL] = {
        .label = _("Outlay"),
        .type = CT_MONEY,
        .get = [doc](int row) -> wxString { return doc->GetOutlayString(row); },
        .set = [this](int row, const wxString &value) -> void {
            m_doc->SetValueString(row, DataDoc::REAL_AMOUNT_COL, value);
            CacheCell(row, INCOME_COL);
            UpdateDocAndCache(row);
        },
    };
    SetColImpl(_("Item"), REAL_DESC_COL, DataDoc::REAL_DESC_COL);
    m_colImpls[BALANCE_COL] = {
        .label = _("Balance"),
        .type = CT_MONEY,
        .get = [doc](int row) -> wxString { return doc->GetBalanceString(row); },
        .set = nullptr,
    };
    SetColImpl(_("Memo"), MEMO_COL, DataDoc::MEMO_COL);
    m_colImpls[CATEGORY_COL] = {
        .label = _("Category"),
        .type = CT_IGNORE,
        .get = nullptr,
        .set = nullptr,
    };
    m_cache.resize(m_doc->GetRowCount());
    SetAttrProvider(new DataGridCellAttrProvider(this));
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
