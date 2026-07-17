#include <istream>

#include <wx/log.h>
#include <wx/translation.h>

#include <string>

#include "ImportDoc.h"

#include "../data/DataDoc.h"

#include "csv/date_time.h"
#include "csv/str.h"

std::map<wxString, int> ImportDoc::s_colMap = {
    {     "Date",         DataDoc::DATE_COL},
    {     "Time",         DataDoc::TIME_COL},
    {     "Desc",         DataDoc::DESC_COL},
    {   "Amount",       DataDoc::AMOUNT_COL},
    { "DateTime",   ImportDoc::DATETIME_COL},
    {      "Dir",        ImportDoc::DIR_COL},
    {"AbsAmount", ImportDoc::ABS_AMOUNT_COL},
};

ImportDoc::ImportDoc() : HaCsvTemplate<ImportDoc>(), m_colTitles(), m_types(nullptr), m_csvColDataFieldMap()
{
    wxLog::AddTraceMask(TM);
}

ImportDoc::~ImportDoc()
{
    if (m_types != nullptr) {
        delete[] m_types;
    }
}

date_t ImportDoc::GetRecordDate(const record_t *record) const
{
    auto csvCol = m_csvColDataFieldMap.v_k(DataDoc::DATE_COL);
    if (csvCol == INVALID_COL) {
        return UNKNOWN_DATE;
    }
    return *(date_t *)get_const_field(&m_parser, record, csvCol);
}

timo_t ImportDoc::GetRecordTime(const record_t *record) const
{
    auto csvCol = m_csvColDataFieldMap.v_k(DataDoc::TIME_COL);
    if (csvCol == INVALID_COL) {
        return UNKNOWN_TIME;
    }
    return *(timo_t *)get_const_field(&m_parser, record, csvCol);
}

std::pair<date_t, timo_t> ImportDoc::GetRecordDateTime(const record_t *record) const
{
    auto date = GetRecordDate(record);
    auto time = GetRecordTime(record);
    if (date != UNKNOWN_DATE) {
        return {date, time};
    }
    auto csvCol = m_csvColDataFieldMap.v_k(ImportDoc::DATETIME_COL);
    if (csvCol == INVALID_COL) {
        return {UNKNOWN_DATE, UNKNOWN_TIME};
    }
    auto *s = (const struct str *)get_const_field(&m_parser, record, csvCol);
    if (str_is_empty(s)) {
        return {UNKNOWN_DATE, UNKNOWN_TIME};
    }
    auto *p = parse_date(s->buf, &date, ' ', m_parser.options.date_sep);
    parse_time(p, &time, m_parser.options.sep);
    return {date, time};
}

wxString ImportDoc::GetCsvTitle(int i) const
{
    wxASSERT(0 <= i && (size_t)i < m_colTitles.size());
    return m_colTitles[i];
}

int ImportDoc::GetDataField(int i) const
{
    return m_csvColDataFieldMap.k_v(i);
}

bool ImportDoc::DateColExists() const
{
    return m_csvColDataFieldMap.has_v(DataDoc::DATE_COL) || m_csvColDataFieldMap.has_v(ImportDoc::DATETIME_COL);
}

int ImportDoc::Reading(std::istream &is)
{
    std::string line;
    std::getline(is, line);
    if (is.fail()) {
        wxLogError(_("Empty CSV input"));
        return false;
    }
    const char *p = line.c_str();
    int cols = parse_count(p, m_parser.options.sep);
    if (cols <= 0) {
        wxLogError(_("Invalid CSV header"));
        return false;
    }
    for (int i = 0; i < cols; ++i) {
        struct str s;
        p = parse_str(p, &s, m_parser.options.sep);
        m_colTitles.push_back(wxString(s.buf, s.len));
        ++p;
    }

    m_types = new enum column_type[cols];
    std::fill(m_types, m_types + cols, CT_STR);

    for (int i = 0; i < cols; ++i) {
        try {
            int field = ImportDoc::s_colMap.at(m_colTitles[i]);
            m_csvColDataFieldMap.set_kv(i, field);
            if (field < DataDoc::COLS) {
                m_types[i] = DataDoc::COL_TYPES[field];
            } else if (field == ImportDoc::ABS_AMOUNT_COL) {
                m_types[i] = CT_MONEY;
            }
        } catch (const std::out_of_range &) {
            // Ignore missing columns
        }
    }
    SetParser(cols, m_types, 0);
    int lines = HaCsv::Reading(is);
    if (lines < 0) {
        return lines - 1;
    }
    return lines + 1;
}

int ImportDoc::Writing(std::ostream &os)
{
    for (size_t i = 0; i < m_colTitles.size(); ++i) {
        if (i > 0) {
            os.put(',');
        }
        auto s = w2s(m_colTitles[i]);
        os.write(s.c_str(), s.length());
    }
    os.put('\n');
    return HaCsv::Writing(os) + 1;
}
