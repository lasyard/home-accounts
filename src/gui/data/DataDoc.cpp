#include <wx/log.h>
#include <wx/translation.h>

#include "DataDoc.h"

#include "csv/date_time.h"

IMPLEMENT_TM(DataDoc)

const column_type DataDoc::COL_TYPES[] = {
    CT_DATE,
    CT_TIME,
    CT_MONEY,
    CT_INT,
    CT_STR,
    CT_MONEY,
    CT_STR,
    CT_STR,
    CT_BOOL,
};

DataDoc::DataDoc(int year) : CsvDoc(COLS, COL_TYPES, 1), m_year(year)
{
    wxLog::AddTraceMask(TM);
}

DataDoc::~DataDoc()
{
}

void DataDoc::SetOpening(money_t opening)
{
    m_stat.opening = opening;
    UpdateBalanceStat();
}

void DataDoc::UpdateBalanceStat()
{
    m_stat.income = 0;
    m_stat.outlay = 0;
    money_t balance = m_stat.opening;
    for (struct list_item *pos = m_records.first; pos != NULL; pos = pos->next) {
        record_t *record = get_record(pos);
        auto amount = (record->flag == RECORD_FLAG_NORMAL ? GetRecordRealAmount(record) : 0);
        if (amount < 0) {
            m_stat.income -= amount;
        } else if (amount > 0) {
            m_stat.outlay += amount;
        }
        SetRecordBalance(record, balance = balance - amount);
    }
    m_stat.closing = balance;
}

bool DataDoc::AfterRead()
{
    fill_serial(&m_parser, &m_records, jdn(m_year, 1, 1), jdn(m_year, 12, 31));
    SetOpening(0);
    return CsvDoc::AfterRead();
}

bool DataDoc::IsRecordEmpty(record_t *record)
{
    if (record->flag == RECORD_FLAG_COMMENT) {
        return record->list.next == NULL || get_record(record->list.next)->flag == RECORD_FLAG_COMMENT;
    } else {
        return false;
    }
}
