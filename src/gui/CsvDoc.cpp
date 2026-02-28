#include <wx/log.h>
#include <wx/translation.h>

#include <sstream>

#include "CsvDoc.h"

#include "csv/str.h"

IMPLEMENT_TM(CsvDoc)

CsvDoc::CsvDoc() : m_accessors(nullptr)
{
    wxLog::AddTraceMask(TM);
    init_parser(&m_parser);
    list_head_init(&m_records);
}

CsvDoc::~CsvDoc()
{
    delete[] m_accessors;
    release_records(&m_parser, &m_records);
    release_parser(&m_parser);
}

const wxString CsvDoc::GetValueString(int pos, int i) const
{
    wxASSERT(m_accessors != nullptr);
    record_t *record = GetRecord(pos);
    wxASSERT(is_index_valid(&m_parser, record, i));
    auto *getter = m_accessors[i].get;
    if (getter != nullptr) {
        return getter(&m_parser, record, i);
    }
    return wxEmptyString;
}

void CsvDoc::SetValueString(int pos, int i, const wxString &value)
{
    wxASSERT(m_accessors != nullptr);
    record_t *record = GetRecord(pos);
    wxASSERT(is_index_valid(&m_parser, record, i));
    auto *setter = m_accessors[i].set;
    if (setter != nullptr) {
        setter(&m_parser, record, i, value);
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

void CsvDoc::SetParser(int cols, const enum column_type types[], int comment_cols)
{
    set_parser_types(&m_parser, cols, types, comment_cols);
    list_head_init(&m_records);
    m_accessors = new struct Accessor[cols];
    for (int i = 0; i < cols; ++i) {
        if (m_parser.meta->types[i] != CT_STR) {
            m_accessors[i].get = &CsvDoc::DefaultGetter;
        } else {
            m_accessors[i].get = &CsvDoc::StrGetter;
        }
        m_accessors[i].set = &CsvDoc::DefaultSetter;
    }
}

bool CsvDoc::ReadStream(std::istream &is)
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

bool CsvDoc::WriteStream(std::ostream &os)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    if (!BeforeWrite()) {
        wxLogStatus(_("%d lines written"), 0);
        return false;
    }
    int lines = write_lines(&m_parser, &m_records, ::put_line_to_ostream, static_cast<void *>(&os));
    wxLogStatus(_("%d lines written"), lines);
    // rebuild the contents because `BeforeWrite` may have changed the data
    AfterRead();
    return true;
}

bool CsvDoc::Read(const std::string &str)
{
    std::istringstream is(str);
    return ReadStream(is);
}

bool CsvDoc::Write(std::string &str)
{
    std::ostringstream os;
    if (!WriteStream(os)) {
        return false;
    }
    str = os.str();
    return true;
}

const wxString CsvDoc::DefaultGetter(const struct parser *parser, const record_t *record, int i)
{
    char buf[MAX_LINE_LENGTH + 1];
    char *p = output_field(parser, buf, record, i);
    *p = '\0';
    return wxString(buf);
}

const wxString CsvDoc::StrGetter(const struct parser *parser, const record_t *record, int i)
{
    auto *s = (struct str *)get_const_field(parser, record, i);
    if (!str_is_empty(s)) {
        return wxString(s->buf, s->len);
    }
    return wxEmptyString;
}

void CsvDoc::DefaultSetter(const struct parser *parser, record_t *record, int i, const wxString &value)
{
    if (parse_field(parser, value.c_str(), record, i) == NULL) {
        wxLogError(_("Invalid value: %s"), value);
    }
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
