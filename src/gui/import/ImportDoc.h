#ifndef _HA_IMPORT_IMPORT_DOC_H_
#define _HA_IMPORT_IMPORT_DOC_H_

#include "../HaCsvTemplate.h"

#include "../BiMap.h"

#include "csv/date_time.h"

class ImportColMapConf;

class ImportDoc : public HaCsvTemplate<ImportDoc>
{
public:
    DECLARE_TM(ImportDoc)

    ImportDoc();
    virtual ~ImportDoc();

    date_t GetRecordDate(const record_t *record) const;
    timo_t GetRecordTime(const record_t *record) const;
    std::pair<date_t, timo_t> GetRecordDateTime(const record_t *record) const;

    void SetColMap(ImportColMapConf *colMap)
    {
        m_colMap = colMap;
    }

    void SaveColMap(std::string &csv) const;

    wxString GetCsvTitle(int i) const;
    int GetDataField(int i) const;
    wxString GetDataFieldName(int i) const;
    bool SetDataFieldByName(int i, const wxString &name);
    bool DateColExists() const;

    int GetCsvCol(int field) const
    {
        auto csvCol = m_csvColDataFieldMap.v_k(field);
        return csvCol;
    }

private:
    ImportColMapConf *m_colMap;

    wxArrayString m_colTitles;
    enum column_type *m_types;
    BiMap<int, int, INVALID_COL, INVALID_COL> m_csvColDataFieldMap;

    int Reading(std::istream &is) override;
    int Writing(std::ostream &os) override;
};

#endif /* _HA_IMPORT_IMPORT_DOC_H_ */
