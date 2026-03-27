#include <algorithm>
#include <ostream>
#include <vector>

#include <wx/log.h>

#include "ImportColMapConf.h"

#include "csv/str.h"

const column_type ImportColMapConf::COL_TYPES[] = {CT_INT, CT_STR};

ImportColMapConf::ImportColMapConf() : m_titleMap()
{
    wxLog::AddTraceMask(TM);
    SetParser(COLS, COL_TYPES, 0);
}

ImportColMapConf::~ImportColMapConf()
{
}

int ImportColMapConf::GetDataFieldByTitle(const wxString &title) const
{
    if (m_titleMap.contains(title)) {
        return m_titleMap.at(title);
    }
    return INVALID_COL;
}

bool ImportColMapConf::AfterRead()
{
    m_titleMap.clear();
    for (struct list_item *i = m_records.first; i != NULL; i = i->next) {
        auto *record = get_record(i);
        int field = GetRecordField(record);
        auto title = GetRecordTitle(record);
        if (!str_is_empty(title)) {
            m_titleMap[wxString(title->buf, title->len)] = field;
        }
    }
    return CsvDoc::AfterRead();
}

int ImportColMapConf::Writing(std::ostream &os)
{
    std::vector<std::pair<wxString, int>> entries(m_titleMap.begin(), m_titleMap.end());
    std::sort(entries.begin(), entries.end(), [](const auto &lhs, const auto &rhs) {
        if (lhs.second != rhs.second) {
            return lhs.second < rhs.second;
        }
        return lhs.first < rhs.first;
    });
    int count = 0;
    for (const auto &[title, field] : entries) {
        os << field << ',' << w2s(title) << '\n';
        ++count;
    }
    return count;
}
