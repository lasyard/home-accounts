#include <wx/log.h>
#include <wx/translation.h>

#include <sstream>

#include "CsvDoc.h"

#include "csv/str.h"

IMPLEMENT_TM(CsvDoc)

CsvDoc::CsvDoc(int count, const enum column_type types[], int comment_cols)
{
    wxLog::AddTraceMask(TM);
    init_parser(&m_parser);
    set_parser_types(&m_parser, count, types, comment_cols);
    list_head_init(&m_records);
}

CsvDoc::~CsvDoc()
{
    release_records(&m_parser, &m_records);
    release_parser(&m_parser);
}

const wxString CsvDoc::GetValueString(int pos, int i) const
{
    record_t *record = GetRecord(pos);
    if (!is_index_valid(&m_parser, record, i)) {
        return wxEmptyString;
    }
    if (m_parser.meta->types[i] != CT_STR) {
        char buf[MAX_LINE_LENGTH + 1];
        char *p = output_field(&m_parser, buf, record, i);
        *p = '\0';
        return wxString(buf);
    } else {
        auto *s = (struct str *)get_const_field(&m_parser, record, i);
        return wxString(!str_is_empty(s) ? wxString(s->buf, s->len) : wxString(wxEmptyString));
    }
}

void CsvDoc::SetValueString(int pos, int i, const wxString &value)
{
    record_t *record = GetRecord(pos);
    wxASSERT(is_index_valid(&m_parser, record, i));
    if (parse_field(&m_parser, value.c_str(), record, i) == NULL) {
        wxLogError(_("Invalid value: %s"), value);
    }
}

wxString CsvDoc::GetMoneyString(money_t m) const
{
    char buf[MAX_LINE_LENGTH + 1];
    char *p = output_money(buf, m, m_parser.options.money_prec, m_parser.options.money_scale);
    *p = '\0';
    return wxString(buf);
}

record_t *CsvDoc::AddRecord()
{
    record_t *record = new_record(&m_parser);
    return_null_if_null(record);
    if (m_records.last != NULL) {
        record = copy_comment_fields(&m_parser, record, get_record(m_records.last));
        return_null_if_null(record);
    }
    list_add(&m_records, &record->list);
    m_index.push_back(record);
    return record;
}

record_t *CsvDoc::InsertRecord(int pos)
{
    record_t *record = new_record(&m_parser);
    return_null_if_null(record);
    if (pos == 0) {
        list_ins_first(&m_records, &record->list);
        m_index.insert(m_index.begin(), record);
    } else {
        record_t *prev = GetRecord(pos - 1);
        record = copy_comment_fields(&m_parser, record, prev);
        return_null_if_null(record);
        list_ins(&m_records, &prev->list.next, &record->list);
        m_index.insert(std::next(m_index.begin(), pos), record);
    }
    SetNewRecord(record);
    return record;
}

bool CsvDoc::DeleteRecord(int pos)
{
    struct list_item *item = NULL;
    if (pos == 0) {
        item = list_del_first(&m_records);
    } else {
        record_t *p = GetRecord(pos - 1);
        item = list_del(&m_records, &p->list.next);
    }
    if (item != NULL) {
        m_index.erase(std::next(m_index.begin(), pos));
        free_record(&m_parser, get_record(item));
        return true;
    }
    return false;
}

extern "C" int get_line_from_istream(char *buf, size_t len, void *context)
{
    std::istream *is = static_cast<std::istream *>(context);
    if (is->eof()) {
        return -1;
    }
    is->getline(buf, len);
    return is->gcount();
}

bool CsvDoc::Read(std::istream &is)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    release_records(&m_parser, &m_records);
    int lines = read_lines(&m_parser, &m_records, ::get_line_from_istream, static_cast<void *>(&is));
    if (lines < 0) {
        wxLogError(_("Parse error at line %d"), -lines);
        return false;
    }
    wxLogStatus(_("%d lines read"), lines);
    return AfterRead();
}

extern "C" int put_line_to_ostream(const char *buf, size_t len, void *context)
{
    std::ostream *os = static_cast<std::ostream *>(context);
    os->write(buf, len);
    os->put('\n');
    return len;
}

bool CsvDoc::Write(std::ostream &os)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    if (!BeforeWrite()) {
        wxLogStatus(_("%d lines written"), 0);
        return false;
    }
    int lines = write_lines(&m_parser, &m_records, ::put_line_to_ostream, static_cast<void *>(&os));
    wxLogStatus(_("%d lines written"), lines);
    return true;
}

bool CsvDoc::Read(const std::string &str)
{
    std::istringstream is(str);
    return Read(is);
}

bool CsvDoc::Write(std::string &str)
{
    std::ostringstream os;
    if (!Write(os)) {
        return false;
    }
    str = os.str();
    return true;
}

void CsvDoc::CreateIndex()
{
    m_index.clear();
    for (struct list_item *pos = m_records.first; pos != NULL; pos = pos->next) {
        record_t *record = get_record(pos);
        m_index.push_back(record);
    }
}

bool CsvDoc::AfterRead()
{
    CreateIndex();
    return true;
}

bool CsvDoc::BeforeWrite()
{
    struct list_item **pos = &m_records.first;
    while (*pos != NULL) {
        record_t *record = get_record(*pos);
        if (IsRecordEmpty(record)) {
            list_del(&m_records, pos);
            free_record(&m_parser, record);
        } else {
            pos = &(*pos)->next;
        }
    }
    return true;
}

void CsvDoc::SetNewRecord([[maybe_unused]] record_t *record)
{
}

bool CsvDoc::IsRecordEmpty([[maybe_unused]] record_t *record)
{
    return false;
}
