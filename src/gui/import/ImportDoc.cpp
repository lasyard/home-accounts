#include <istream>

#include <wx/log.h>
#include <wx/translation.h>

#include <string>

#include "ImportDoc.h"

#include "ImportColMapConf.h"

#include "../data/DataDoc.h"

#include "csv/date_time.h"
#include "csv/str.h"

ImportDoc::ImportDoc()
    : HaCsvTemplate<ImportDoc>()
    , m_colMap(nullptr)
    , m_colTitles()
    , m_types(nullptr)
    , m_csvColDataFieldMap()
{
    wxLog::AddTraceMask(TM);
}

ImportDoc::~ImportDoc()
{
    if (m_colMap != nullptr) {
        delete m_colMap;
    }
    if (m_types != nullptr) {
        delete[] m_types;
    }
}

void ImportDoc::SaveColMap(std::string &csv) const
{
    if (m_colMap != nullptr) {
        m_colMap->Write(csv);
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
    auto csvCol = m_csvColDataFieldMap.v_k(DataDoc::DATETIME_VIRTUAL_COL);
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

wxString ImportDoc::GetDataFieldName(int i) const
{
    return DataDoc::GetColName(GetDataField(i));
}

bool ImportDoc::SetDataFieldByName(int csvCol, const wxString &name)
{
    wxASSERT(0 <= csvCol && (size_t)csvCol < m_colTitles.size());
    int field = DataDoc::GetColByName(name);
    if (field != INVALID_COL) {
        auto oldCol = m_csvColDataFieldMap.v_k(field);
        if (oldCol != INVALID_COL) {
            m_colMap->SetMap(INVALID_COL, m_colTitles[oldCol]);
        }
    }
    m_colMap->SetMap(field, m_colTitles[csvCol]);
    if (field == DataDoc::DATETIME_VIRTUAL_COL) {
        auto oldK = m_csvColDataFieldMap.erase_v(DataDoc::DATE_COL);
        if (oldK != INVALID_COL) {
            m_colMap->SetMap(INVALID_COL, m_colTitles[oldK]);
        }
        oldK = m_csvColDataFieldMap.erase_v(DataDoc::TIME_COL);
        if (oldK != INVALID_COL) {
            m_colMap->SetMap(INVALID_COL, m_colTitles[oldK]);
        }
    }
    return true;
}

bool ImportDoc::DateColExists() const
{
    return m_csvColDataFieldMap.has_v(DataDoc::DATE_COL) || m_csvColDataFieldMap.has_v(DataDoc::DATETIME_VIRTUAL_COL);
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

    if (m_colMap != nullptr) {
        for (int i = 0; i < cols; ++i) {
            int field = m_colMap->GetDataFieldByTitle(m_colTitles[i]);
            if (field != INVALID_COL) {
                m_csvColDataFieldMap.set_kv(i, field);
                if (field != DataDoc::DATETIME_VIRTUAL_COL) {
                    m_types[i] = DataDoc::COL_TYPES[field];
                }
            }
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
