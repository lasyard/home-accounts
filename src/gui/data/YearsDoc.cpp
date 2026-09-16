#include "YearsDoc.h"

#include "DataDoc.h"

YearsDoc::YearsDoc() : HaCsv()
{
    SetParser(COLS, COL_TYPES, COL_TITLES);
}

YearsDoc::~YearsDoc()
{
}

void YearsDoc::SetSummary(const DataDoc *data)
{
    year_t year = data->GetYear();
    record_t *target = nullptr;
    for (size_t row = 0; row < GetRowCount(); ++row) {
        auto *record = GetRecord(row);
        year_t recordYear = GetRecordYear(record);
        if (recordYear == year) {
            target = record;
            break;
        } else if (recordYear > year) {
            target = InsertRecord(row);
            break;
        }
    }
    if (target == nullptr) {
        target = AddRecord();
    }
    *(year_t *)get_field(&m_parser, target, YEAR_COL) = year;
    *(money_t *)get_field(&m_parser, target, INCOME_COL) = data->GetStatIncome();
    *(money_t *)get_field(&m_parser, target, OUTLAY_COL) = data->GetStatOutlay();
    *(int64_t *)get_field(&m_parser, target, RECORDS_COL) = data->GetRecordCount();
}

money_t YearsDoc::CalcBalance(year_t year) const
{
    money_t balance = 0;
    for (struct list_item *pos = m_records.first; pos != nullptr; pos = pos->next) {
        auto *record = get_record(pos);
        year_t recordYear = GetRecordYear(record);
        if (recordYear >= year) {
            break;
        }
        balance += GetRecordIncome(record) - GetRecordOutlay(record);
    }
    return balance;
}
