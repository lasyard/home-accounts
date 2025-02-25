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
              TT("memo"),
              TT("category"),
          }
      )
{
    SetColImpl(m_colImpl[0], DATA_TIME_COL);
    SetColImpl(m_colImpl[1], DATA_AMOUNT_COL);
    SetColImpl(m_colImpl[2], DATA_ACCOUNT_COL);
    SetColImpl(m_colImpl[3], DATA_DESC_COL);
    m_colImpl[4].type = m_doc->GetItemValueType(DATA_REAL_AMOUNT_COL);
    m_colImpl[4].get = [this](int row) -> wxString {
        return HaTable::GetItemCellMoneyValueBySign(row, DATA_REAL_AMOUNT_COL, true);
    };
    m_colImpl[4].set = [this](int row, const wxString &value) -> void {
        HaTable::SetItemCellValue(row, DATA_REAL_AMOUNT_COL, "-" + value);
        CacheCell(row, 5);
    };
    m_colImpl[5].type = m_doc->GetItemValueType(DATA_REAL_AMOUNT_COL);
    m_colImpl[5].get = [this](int row) -> wxString {
        return HaTable::GetItemCellMoneyValueBySign(row, DATA_REAL_AMOUNT_COL, false);
    };
    m_colImpl[5].set = [this](int row, const wxString &value) -> void {
        HaTable::SetItemCellValue(row, DATA_REAL_AMOUNT_COL, value);
        CacheCell(row, 4);
    };
    SetColImpl(m_colImpl[6], DATA_REAL_DESC_COL);
    SetColImpl(m_colImpl[7], DATA_MEMO_COL);
    UnsetColImpl(m_colImpl[8]);
}

HaDataTable::~HaDataTable()
{
}

void HaDataTable::Init()
{
    HaTable::Init();
    SetAttrProvider(new HaDataGridCellAttrProvider(this));
}

void HaDataTable::SetColImpl(struct ColImpl &colImpl, int col)
{
    colImpl.type = m_doc->GetItemValueType(col);
    colImpl.get = [this, col](int row) -> wxString { return HaTable::GetItemCellValue(row, col); };
    colImpl.set = [this, col](int row, const wxString &value) -> void {
        return HaTable::SetItemCellValue(row, col, value);
    };
}

void HaDataTable::UnsetColImpl(struct ColImpl &colImpl)
{
    colImpl.type = CT_IGNORE;
    colImpl.get = [](int) -> wxString { return _("not implemented"); };
    colImpl.set = [](int, const wxString &) -> void {};
}

const wxString HaDataTable::GetItemCellValue(int row, int col)
{
    if (col < COLUMNS) {
        return m_colImpl[col].get(row);
    }
    return wxEmptyString;
}

void HaDataTable::SetItemCellValue(int row, int col, const wxString &value)
{
    if (col < COLUMNS) {
        m_colImpl[col].set(row, value);
    }
}
