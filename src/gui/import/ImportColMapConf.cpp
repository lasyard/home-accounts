#include <wx/log.h>
#include <wx/tokenzr.h>

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
    return INVALID_DATA_FIELD;
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
