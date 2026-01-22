#include "DataTable.h"

#include "DataDoc.h"
#include "DataGridCellAttrProvider.h"

#include "../Utils.h"

IMPLEMENT_DYNAMIC_CLASS(DataTable, HaTable)

DataTable::DataTable(DataDoc *doc)
    : HaTable(
          {
              _("Time"),
              _("Amount"),
              _("Account"),
              _("Description"),
              _("Income"),
              _("Outlay"),
              _("Item"),
              _("Balance"),
              _("Memo"),
              _("Category"),
          },
          doc
      )
{
    SetAttrProvider(new DataGridCellAttrProvider(this));
    MapColToCol(0, DataDoc::TIME_COL);
    MapColToCol(1, DataDoc::AMOUNT_COL);
    MapColToCol(2, DataDoc::ACCOUNT_COL);
    MapColToCol(3, DataDoc::DESC_COL);
    m_colImpls[INCOME_COL] = {
        .type = CT_MONEY,
        .get = [this](int row) -> wxString { return GetDataDoc()->GetIncomeString(row); },
        .set = [this](int row, const wxString &value) -> void {
            m_doc->SetValueString(row, DataDoc::REAL_AMOUNT_COL, "-" + value);
            CacheCell(row, OUTLAY_COL);
            UpdateDocAndCache(row);
        },
    };
    m_colImpls[OUTLAY_COL] = {
        .type = CT_MONEY,
        .get = [this](int row) -> wxString { return GetDataDoc()->GetOutlayString(row); },
        .set = [this](int row, const wxString &value) -> void {
            m_doc->SetValueString(row, DataDoc::REAL_AMOUNT_COL, value);
            CacheCell(row, INCOME_COL);
            UpdateDocAndCache(row);
        },
    };
    MapColToCol(6, DataDoc::REAL_DESC_COL);
    m_colImpls[BALANCE_COL] = {
        .type = CT_MONEY,
        .get = [this](int row) -> wxString { return GetDataDoc()->GetBalanceString(row); },
        .set = nullptr,
    };
    MapColToCol(8, DataDoc::MEMO_COL);
    m_colImpls[9] = {
        .type = CT_IGNORE,
        .get = nullptr,
        .set = nullptr,
    };
}

DataTable::~DataTable()
{
}

DataDoc *DataTable::GetDataDoc()
{
    return static_cast<DataDoc *>(m_doc);
}

void DataTable::OnNewRow(size_t pos)
{
    HaTable::OnNewRow(pos);
    UpdateDocAndCache(pos);
}

void DataTable::UpdateDocAndCache(int row)
{
    GetDataDoc()->UpdateBalanceStat();
    for (auto i = row; i < GetNumberRows(); ++i) {
        CacheCell(i, BALANCE_COL);
    }
}
