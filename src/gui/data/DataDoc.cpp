#include <algorithm>

#include <wx/log.h>
#include <wx/translation.h>

#include "DataDoc.h"

#include "csv/date_time.h"

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

DataDoc::DataDoc(int year) : HaCsvTemplate<DataDoc>(), m_year(year)
{
    wxLog::AddTraceMask(TM);
    SetParser(COLS, COL_TYPES, 1);
    SetAccessor(ACCOUNT_COL, CT_STR, &DataDoc::AccountGetter, &DataDoc::AccountSetter);
}

DataDoc::~DataDoc()
{
}

wxString DataDoc::GetColName(int i)
{
    static wxArrayString s;
    if (0 <= i && i < COLS) {
        if (s.IsEmpty()) {
            s.Add(_("Date"));
            s.Add(_("Time"));
            s.Add(_("Amount"));
            s.Add(_("Account"));
            s.Add(_("Description"));
            s.Add(_("Real Amount"));
            s.Add(_("Real Description"));
            s.Add(_("Memo"));
            s.Add(_("Auto Set"));
        }
        return s[i];
    }
    return _("Invalid");
}

int DataDoc::GetColByName(const wxString &name)
{
    for (int i = 0; i < COLS; ++i) {
        if (GetColName(i) == name) {
            return i;
        }
    }
    return INVALID_COL;
}

void DataDoc::SetAccountNames(const std::vector<int64_t> &ids, const wxArrayString &names)
{
    m_accountNames = names;
    m_accountIdNames.clear();
    m_accountNameIds.clear();
    auto n = ids.size();
    for (size_t i = 0; i < n; ++i) {
        auto id = ids[i];
        auto &name = names[i];
        m_accountIdNames[id] = name;
        m_accountNameIds[name] = id;
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

int DataDoc::FindDateRow(int year, int month, int day)
{
    int target_jdn = jdn(year, month, day);
    auto it =
        std::lower_bound(m_index.begin(), m_index.end(), target_jdn, [parser = &m_parser](record_t *record, int jdn) {
            int record_jdn = *(date_t *)get_const_field(parser, record, DATE_COL);
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
            if (record->flag == RECORD_FLAG_COMMENT) {
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

const wxString DataDoc::AccountGetter(const record_t *record, int i) const
{
    auto id = *(int64_t *)get_const_field(&m_parser, record, i);
    auto it = m_accountIdNames.find(id);
    if (it != m_accountIdNames.end()) {
        return it->second;
    }
    return wxString::Format("%lld", id);
}

void DataDoc::AccountSetter(record_t *record, int i, const wxString &value)
{
    auto it = m_accountNameIds.find(value);
    if (it != m_accountNameIds.end()) {
        *(int64_t *)get_field(&m_parser, record, i) = it->second;
    } else if (parse_field(&m_parser, value.c_str(), record, i) == NULL) {
        wxLogError(_("Invalid value: %s"), value);
    }
}

bool DataDoc::AfterRead()
{
    int start = jdn(m_year, 1, 1);
    int end = jdn(m_year, 12, 31);
    fill_serial(&m_parser, &m_records, start, end);
    SetOpening(0);
    return HaCsv::AfterRead();
}

bool DataDoc::IsRecordEmpty(record_t *record)
{
    if (record->flag == RECORD_FLAG_COMMENT) {
        return record->list.next == NULL || get_record(record->list.next)->flag == RECORD_FLAG_COMMENT;
    } else {
        return false;
    }
}
