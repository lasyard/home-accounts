#include <algorithm>

#include <wx/log.h>
#include <wx/translation.h>

#include "DataDoc.h"

#include "../Utils.h"

#include "csv/csv_utils.h"
#include "csv/date_time.h"

DataDoc::DataDoc(int year) : HaData(), m_year(year), m_accountNames(), m_accountIdNameMap()
{
    wxLog::AddTraceMask(TM);
    SetAccessor(HaData::ACCOUNT_COL, CT_STR, &DataDoc::AccountGetter, &DataDoc::AccountSetter);
}

DataDoc::~DataDoc()
{
}

void DataDoc::SetAccountIdAndNames(const std::vector<int64_t> &ids, const wxArrayString &names)
{
    m_accountNames = names;
    m_accountIdNameMap.clear();
    for (size_t i = 0; i < ids.size(); ++i) {
        m_accountIdNameMap.set_kv(ids[i], names[i]);
    }
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
        auto amount = (record->flag == RECORD_FLAG_NORMAL ? GetRecordAmount(record) : 0);
        if (amount < 0) {
            m_stat.income -= amount;
        } else if (amount > 0) {
            m_stat.outlay += amount;
        }
        SetRecordBalance(record, balance = balance - amount);
    }
    m_stat.closing = balance;
}

int DataDoc::FindDateRow(int year, int month, int day)
{
    int target_jdn = jdn(year, month, day);
    auto it =
        std::lower_bound(m_index.begin(), m_index.end(), target_jdn, [parser = &m_parser](record_t *record, int jdn) {
            int record_jdn = *(date_t *)get_const_field(parser, record, HaData::DATE_COL);
            return record_jdn < jdn;
        });
    return it - m_index.begin();
}

record_t *DataDoc::InsertRecordAtTime(date_t date, timo_t time)
{
    size_t pos;
    for (pos = 0; pos < GetRowCount(); ++pos) {
        auto *record = GetRecord(pos);
        auto recordDate = GetRecordDate(record);
        if (date > recordDate) {
            continue;
        }
        if (date == recordDate) {
            if (record->flag == RECORD_FLAG_HASH) {
                continue;
            }
            auto recordTime = GetRecordTime(record);
            if (recordTime == UNKNOWN_TIME) {
                continue;
            }
            if (time != UNKNOWN_TIME && time >= recordTime) {
                continue;
            }
        }
        break;
    }
    auto *record = InsertRecord(pos);
    if (record == nullptr) {
        return nullptr;
    }
    SetRecordDate(record, date);
    SetRecordTime(record, time);
    return record;
}

const wxString DataDoc::AccountGetter(const HaCsv *csv, const record_t *record, int i)
{
    auto *dataDoc = static_cast<const DataDoc *>(csv);
    auto id = *(int64_t *)get_const_field(&dataDoc->m_parser, record, i);
    auto name = dataDoc->m_accountIdNameMap.k_v(id);
    if (name != HaData::INVALID_COL_NAME) {
        return name;
    }
    return wxString::Format("%lld", id);
}

void DataDoc::AccountSetter(HaCsv *csv, record_t *record, int i, const wxString &value)
{
    auto *dataDoc = static_cast<DataDoc *>(csv);
    auto id = dataDoc->m_accountIdNameMap.v_k(value);
    if (id != HaData::INVALID_COL) {
        *(int64_t *)get_field(&dataDoc->m_parser, record, i) = id;
    } else if (parse_field(&dataDoc->m_parser, value.c_str(), record, i) == NULL) {
        wxLogError(_("Invalid value: %s"), value);
    }
}

bool DataDoc::AfterRead()
{
    set_hash_cols(&m_parser, 1);
    int start = jdn(m_year, 1, 1);
    int end = jdn(m_year, 12, 31);
    fill_serial(&m_parser, &m_records, start, end, ::record_is_hash, ::get_record_first_int32, ::new_hash_record_int32);
    SetOpening(0);
    return HaData::AfterRead();
}
