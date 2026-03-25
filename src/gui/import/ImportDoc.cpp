#include <istream>

#include <wx/arrstr.h>
#include <wx/log.h>
#include <wx/translation.h>

#include <string>

#include "ImportDoc.h"

#include "ImportColMapConf.h"

#include "../data/DataDoc.h"

#include "csv/str.h"

ImportDoc::ImportDoc() : m_colMap(nullptr), m_colTitles(), m_types(nullptr), m_dataFields(nullptr)
{
    wxLog::AddTraceMask(TM);
}

ImportDoc::~ImportDoc()
{
    if (m_types != nullptr) {
        delete[] m_types;
    }
    if (m_dataFields != nullptr) {
        delete[] m_dataFields;
    }
}

void ImportDoc::SetColumnMap(const std::string &str)
{
    m_colMap.reset();
    if (str.empty()) {
        return;
    }
    auto colMap = std::make_unique<ImportColMapConf>();
    if (!colMap->Read(str)) {
        wxLogError(_("Invalid import field mapping"));
        return;
    }
    m_colMap = std::move(colMap);
}

wxString ImportDoc::GetCsvTitle(int i) const
{
    wxASSERT(0 <= i && (size_t)i < m_colTitles.size());
    return m_colTitles[i];
}

wxString ImportDoc::GetDataFieldName(int i) const
{
    int field = GetDataField(i);
    return field != ImportColMapConf::INVALID_DATA_FIELD ? DataDoc::GetColName(field) : wxString();
}

int ImportDoc::GetDataField(int i) const
{
    if (0 <= i && (size_t)i < m_colTitles.size()) {
        return m_dataFields[i];
    }
    return ImportColMapConf::INVALID_DATA_FIELD;
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
    std::fill(m_dataFields, m_dataFields + cols, ImportColMapConf::INVALID_DATA_FIELD);

    if (m_colMap != nullptr) {
        for (int i = 0; i < cols; ++i) {
            int field = m_colMap->GetDataFieldByTitle(m_colTitles[i]);
            if (field >= 0) {
                m_dataFields[i] = field;
                m_types[i] = DataDoc::COL_TYPES[field];
            }
        }
    }
    SetParser(cols, m_types, 0);
    int lines = CsvDoc::Reading(is);
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
    return CsvDoc::Writing(os) + 1;
}
