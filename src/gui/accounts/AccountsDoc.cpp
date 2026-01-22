#include <wx/log.h>
#include <wx/translation.h>

#include "AccountsDoc.h"

#include "csv/str.h"

IMPLEMENT_TM(AccountsDoc)

const column_type AccountsDoc::COL_TYPES[] = {
    CT_INT,
    CT_INT,
    CT_STR,
    CT_STR,
    CT_DATE,
    CT_MONEY,
    CT_STR,
};

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

AccountsDoc::AccountsDoc() : CsvDoc(COLS, COL_TYPES, 1), m_maxId(0)
{
    wxLog::AddTraceMask(TM);
}

AccountsDoc::~AccountsDoc()
{
}

const wxString AccountsDoc::GetValueString(int pos, int i) const
{
    if (i == TYPE_COL) {
        int64_t type = GetRecordType(pos);
        const wxArrayString &types = GetAccountTypeStrings();
        if (type < 0 || type >= (int64_t)types.size()) {
            type = 0;
        }
        return types[type];
    }
    return CsvDoc::GetValueString(pos, i);
}

void AccountsDoc::SetValueString(int pos, int i, const wxString &value)
{
    if (i == TYPE_COL) {
        int64_t index;
        const wxArrayString &types = GetAccountTypeStrings();
        for (index = 0; index < (int64_t)types.size(); ++index) {
            if (types[index] == value) {
                break;
            }
        }
        if (index == (int64_t)types.size()) {
            index = 0;
        }
        SetRecordType(pos, index);
    } else {
        CsvDoc::SetValueString(pos, i, value);
    }
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
    fill_serial(&m_parser, &m_records, 1, 3);
    return CsvDoc::AfterRead();
}

void AccountsDoc::SetNewRecord(record_t *record)
{
    SetRecordId(record, ++m_maxId);
}

bool AccountsDoc::IsRecordEmpty(record_t *record)
{
    return record->flag == RECORD_FLAG_NORMAL && str_is_empty(GetRecordName(record));
}
