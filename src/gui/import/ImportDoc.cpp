#include <istream>

#include <wx/log.h>
#include <wx/translation.h>

#include <string>

#include "ImportDoc.h"

#include "ImportColMapConf.h"

#include "../data/DataDoc.h"

#include "csv/str.h"

ImportDoc::ImportDoc()
    : HaCsvTemplate<ImportDoc>()
    , m_colMap(nullptr)
    , m_colTitles()
    , m_types(nullptr)
    , m_dataFields(nullptr)
    , m_csvCols(new int[DataDoc::COLS])
{
    wxLog::AddTraceMask(TM);
    std::fill(m_csvCols, m_csvCols + DataDoc::COLS, INVALID_COL);
}

ImportDoc::~ImportDoc()
{
    if (m_colMap != nullptr) {
        delete m_colMap;
    }
    if (m_types != nullptr) {
        delete[] m_types;
    }
    if (m_dataFields != nullptr) {
        delete[] m_dataFields;
    }
    if (m_csvCols != nullptr) {
        delete[] m_csvCols;
    }
}

date_t ImportDoc::GetRecordDate(const record_t *record) const
{
    int i = m_csvCols[DataDoc::DATE_COL];
    return i != INVALID_COL ? *(date_t *)get_const_field(&m_parser, record, i) : UNKNOWN_DATE;
}

timo_t ImportDoc::GetRecordTime(const record_t *record) const
{
    int i = m_csvCols[DataDoc::TIME_COL];
    return i != INVALID_COL ? *(timo_t *)get_const_field(&m_parser, record, i) : UNKNOWN_TIME;
}

wxString ImportDoc::GetCsvTitle(int i) const
{
    wxASSERT(0 <= i && (size_t)i < m_colTitles.size());
    return m_colTitles[i];
}

int ImportDoc::GetDataField(int i) const
{
    return (0 <= i && (size_t)i < m_colTitles.size()) ? m_dataFields[i] : INVALID_COL;
}

wxString ImportDoc::GetDataFieldName(int i) const
{
    return DataDoc::GetColName(GetDataField(i));
}

bool ImportDoc::SetDataFieldByName(int i, const wxString &name)
{
    if (!(0 <= i && (size_t)i < m_colTitles.size())) {
        return false;
    }
    int oldField = m_dataFields[i];
    if (oldField != INVALID_COL) {
        m_csvCols[oldField] = INVALID_COL;
    }
    int field = DataDoc::GetColByName(name);
    if (field != INVALID_COL) {
        int col = m_csvCols[field];
        if (col != INVALID_COL) {
            m_dataFields[col] = INVALID_COL;
            m_colMap->SetMap(m_dataFields[col], m_colTitles[col]);
        }
        m_csvCols[field] = i;
    }
    m_dataFields[i] = field;
    m_colMap->SetMap(field, m_colTitles[i]);
    return true;
}

int ImportDoc::GetCsvCol(int i) const
{
    return (0 <= i && (size_t)i < DataDoc::COLS) ? m_csvCols[i] : INVALID_COL;
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
    m_dataFields = new int[cols];
    std::fill(m_dataFields, m_dataFields + cols, INVALID_COL);

    if (m_colMap != nullptr) {
        for (int i = 0; i < cols; ++i) {
            int field = m_colMap->GetDataFieldByTitle(m_colTitles[i]);
            if (field != INVALID_COL) {
                m_csvCols[field] = i;
                m_dataFields[i] = field;
                m_types[i] = DataDoc::COL_TYPES[field];
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
