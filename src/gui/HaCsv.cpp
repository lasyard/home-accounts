#include <wx/log.h>
#include <wx/translation.h>

#include <sstream>

#include "HaCsv.h"

#include "Utils.h"

#include "csv/str.h"

HaCsv::HaCsv() : m_titles(nullptr), m_accessors()
{
    wxLog::AddTraceMask(TM);
    init_parser(&m_parser);
    list_head_init(&m_records);
}

HaCsv::~HaCsv()
{
    release_records(&m_parser, &m_records);
    release_parser(&m_parser);
}

wxString HaCsv::GetColTitle(int i) const
{
    wxASSERT(i < GetColCount());
    return wxString(m_titles[i].buf, m_titles[i].len);
}

enum column_type HaCsv::GetColType(int i) const
{
    wxASSERT(i < GetColCount());
    return m_accessors[i].type;
}

wxString HaCsv::GetMoneyString(money_t m) const
{
    char buf[MAX_LINE_LENGTH + 1];
    char *p = output_money(buf, m, m_parser.options.money_prec, m_parser.options.money_scale);
    *p = '\0';
    return wxString(buf);
}

const wxString HaCsv::GetValueString(int pos, int i) const
{
    const record_t *record = GetRecord(pos);
    wxASSERT(record != nullptr && is_index_valid(&m_parser, record, i));
    auto &accessor = m_accessors[i];
    if (accessor.get != nullptr) {
        return accessor.get(this, record, i);
    }
    return wxEmptyString;
}

void HaCsv::SetValueString(int pos, int i, const wxString &value)
{
    record_t *record = GetRecord(pos);
    wxASSERT(record != nullptr && is_index_valid(&m_parser, record, i));
    auto &accessor = m_accessors[i];
    if (accessor.set != nullptr) {
        accessor.set(this, record, i, value);
    }
}

record_t *HaCsv::AddRecord()
{
    record_t *record = new_record(&m_parser);
    return_null_if_null(record);
    if (m_records.last != NULL) {
        record = copy_hash_fields(&m_parser, record, get_record(m_records.last));
        return_null_if_null(record);
    }
    list_add(&m_records, &record->list);
    m_index.push_back(record);
    return record;
}

record_t *HaCsv::InsertRecord(int pos)
{
    record_t *record = new_record(&m_parser);
    return_null_if_null(record);
    if (pos == 0) {
        list_ins_first(&m_records, &record->list);
        m_index.insert(m_index.begin(), record);
    } else {
        record_t *prev = GetRecord(pos - 1);
        if (prev != nullptr) {
            record = copy_hash_fields(&m_parser, record, prev);
            return_null_if_null(record);
        }
        list_ins(&m_records, &prev->list.next, &record->list);
        m_index.insert(std::next(m_index.begin(), pos), record);
    }
    SetNewRecord(record);
    return record;
}

bool HaCsv::DeleteRecord(int pos)
{
    struct list_item *item = NULL;
    if (pos == 0) {
        item = list_del_first(&m_records);
    } else {
        record_t *p = GetRecord(pos - 1);
        wxASSERT(p != nullptr);
        item = list_del(&m_records, &p->list.next);
    }
    if (item != NULL) {
        m_index.erase(std::next(m_index.begin(), pos));
        free_record(&m_parser, get_record(item));
        return true;
    }
    return false;
}

void HaCsv::SetParser(int cols, const enum column_type types[], const struct str titles[])
{
    set_parser_types(&m_parser, cols, types);
    list_head_init(&m_records);
    m_titles = titles;
    m_accessors.resize(cols);
    for (int i = 0; i < cols; ++i) {
        if (m_parser.meta->types[i] != CT_STR) {
            SetAccessor(i, types[i], DefaultGetter, DefaultSetter);
        } else {
            SetAccessor(i, types[i], StrGetter, DefaultSetter);
        }
    }
}

bool HaCsv::ReadStream(std::istream &is)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    release_records(&m_parser, &m_records);
    int lines = Reading(is);
    if (lines < 0) {
        wxLogError(_("Parse error at line %d"), -lines);
        return false;
    }
    wxLogStatus(_("%d lines read"), lines);
    return AfterRead();
}

void HaCsv::WriteStream(std::ostream &os)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    int lines = 0;
    if (BeforeWrite()) {
        lines = Writing(os);
    }
    wxLogStatus(_("%d lines written"), lines);
    // rebuild the contents because `BeforeWrite` may have changed the data
    AfterRead();
}

bool HaCsv::Read(const std::string &str)
{
    std::istringstream is(str);
    return ReadStream(is);
}

void HaCsv::Write(std::string &str)
{
    std::ostringstream os;
    WriteStream(os);
    str = os.str();
}

const wxString HaCsv::DefaultGetter(const HaCsv *csv, const record_t *record, int i)
{
    char buf[MAX_LINE_LENGTH + 1];
    char *p = output_field(&csv->m_parser, buf, record, i);
    *p = '\0';
    return wxString(buf);
}

const wxString HaCsv::StrGetter(const HaCsv *csv, const record_t *record, int i)
{
    auto *s = (struct str *)get_const_field(&csv->m_parser, record, i);
    if (!str_is_empty(s)) {
        return wxString(s->buf, s->len);
    }
    return wxEmptyString;
}

void HaCsv::DefaultSetter(HaCsv *csv, record_t *record, int i, const wxString &value)
{
    if (parse_field(&csv->m_parser, value.c_str(), record, i) == NULL) {
        wxLogError(_("Invalid value: %s"), value);
    }
}

void HaCsv::CreateIndex()
{
    m_index.clear();
    for (struct list_item *pos = m_records.first; pos != NULL; pos = pos->next) {
        record_t *record = get_record(pos);
        m_index.push_back(record);
    }
}

int HaCsv::Reading(std::istream &is)
{
    char buf[MAX_LINE_LENGTH + 1];
    if (m_titles != nullptr && m_parser.meta->cols > 0) {
        if (get_line_from_istream(buf, MAX_LINE_LENGTH, static_cast<void *>(&is)) > 0) {
            if (parse_titles(&m_parser, buf, m_titles) == NULL) {
                return -1;
            }
        }
    }
    if (is.eof()) {
        return 0;
    }
    int line = read_lines(&m_parser, &m_records, ::get_line_from_istream, static_cast<void *>(&is));
    return line + (line > 0 ? 1 : -1); // add 1 for the title line
}

bool HaCsv::AfterRead()
{
    CreateIndex();
    return true;
}

bool HaCsv::BeforeWrite()
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

int HaCsv::Writing(std::ostream &os)
{
    if (list_is_empty(&m_records)) {
        return 0;
    }
    for (int i = 0; i < m_parser.hash_cols; ++i) {
        os.write("#", 1);
    }
    for (int i = 0; i < m_parser.meta->cols; ++i) {
        if (i > 0) {
            os << m_parser.options.sep;
        }
        const struct str *title = &m_titles[i];
        wxASSERT(!str_is_empty(title));
        os.write(title->buf, title->len);
    }
    os.put('\n');
    return write_lines(&m_parser, &m_records, ::put_line_to_ostream, static_cast<void *>(&os));
}

void HaCsv::SetNewRecord([[maybe_unused]] record_t *record)
{
}

bool HaCsv::IsRecordEmpty([[maybe_unused]] record_t *record)
{
    return false;
}
