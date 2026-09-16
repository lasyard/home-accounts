#include <wx/log.h>
#include <wx/translation.h>

#include "AccountsDoc.h"

#include "../Utils.h"

#include "csv/csv_utils.h"
#include "csv/str.h"

const column_type AccountsDoc::COL_TYPES[] = {
    CT_INT,
    CT_INT,
    CT_STR,
    CT_MONEY,
};

const str AccountsDoc::COL_TITLES[] = {
    {   "Type", 4, false},
    {     "Id", 2, false},
    {   "Name", 4, false},
    {"Initial", 7, false},
};

AccountsDoc::AccountsDoc() : HaCsv(), m_maxId(0)
{
    wxLog::AddTraceMask(TM);
    SetParser(COLS, COL_TYPES, COL_TITLES);
    SetAccessor(TYPE_COL, CT_STR, &AccountsDoc::TypeGetter, nullptr);
}

AccountsDoc::~AccountsDoc()
{
}

void AccountsDoc::GetIdAndNames(std::vector<int64_t> &ids, wxArrayString &names) const
{
    for (struct list_item *pos = m_records.first; pos != NULL; pos = pos->next) {
        auto *record = get_record(pos);
        if (record->flag != RECORD_FLAG_NORMAL) {
            continue;
        }
        auto *name = GetRecordName(record);
        if (str_is_empty(name)) {
            continue;
        }
        ids.push_back(GetRecordId(record));
        names.push_back(wxString(name->buf, name->len));
    }
}

const wxString AccountsDoc::TypeGetter(const HaCsv *csv, const record_t *record, int i)
{
    auto type = *(int64_t *)get_const_field(&static_cast<const AccountsDoc *>(csv)->m_parser, record, i);
    const wxArrayString &types = GetAccountTypeStrings();
    if (type < 0 || type >= (int64_t)types.size()) {
        type = 0;
    }
    return types[type];
}

bool AccountsDoc::AfterRead()
{
    m_maxId = 0;
    for (struct list_item *pos = m_records.first; pos != NULL; pos = pos->next) {
        record_t *record = get_record(pos);
        if (record->flag == RECORD_FLAG_NORMAL) {
            int64_t id = GetRecordId(record);
            if (id > m_maxId) {
                m_maxId = id;
            }
        }
    }
    fill_serial(&m_parser, &m_records, 1, 3, ::record_is_hash, ::get_record_first_int64, ::new_hash_record_int64);
    return HaCsv::AfterRead();
}

void AccountsDoc::SetNewRecord(record_t *record)
{
    SetRecordId(record, ++m_maxId);
}

bool AccountsDoc::IsRecordEmpty(record_t *record)
{
    return record->flag == RECORD_FLAG_NORMAL && str_is_empty(GetRecordName(record));
}

const wxArrayString &AccountsDoc::GetAccountTypeStrings()
{
    static wxArrayString s;
    if (s.IsEmpty()) {
        s.Add(_("Invalid"));
        s.Add(_("Debit"));
        s.Add(_("Credit"));
        s.Add(_("Transfer"));
    }
    return s;
}
