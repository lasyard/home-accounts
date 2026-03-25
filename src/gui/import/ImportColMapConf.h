#ifndef _HA_IMPORT_IMPORT_COL_MAP_CONF_H_
#define _HA_IMPORT_IMPORT_COL_MAP_CONF_H_

#include <map>

#include <wx/string.h>

#include "../CsvDoc.h"

class ImportColMapConf : public CsvDoc
{
public:
    DECLARE_TM()

    static constexpr int INVALID_DATA_FIELD = -1;
    static constexpr int COLS = 2;
    static constexpr int FIELD_COL = 0;
    static constexpr int TITLES_COL = 1;

    ImportColMapConf();
    virtual ~ImportColMapConf();

    int GetDataFieldByTitle(const wxString &title) const;

protected:
    bool AfterRead() override;

private:
    static const column_type COL_TYPES[COLS];

    std::map<wxString, int> m_titleMap;

    int GetRecordField(const record_t *record) const
    {
        return *(int64_t *)get_const_field(&m_parser, record, FIELD_COL);
    }

    const struct str *GetRecordTitle(const record_t *record) const
    {
        return (const struct str *)get_const_field(&m_parser, record, TITLES_COL);
    }
};

#endif /* _HA_IMPORT_IMPORT_COL_MAP_CONF_H_ */
