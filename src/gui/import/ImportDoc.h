#ifndef _HA_IMPORT_IMPORT_DOC_H_
#define _HA_IMPORT_IMPORT_DOC_H_

#include "../CsvDoc.h"

#include "csv/date_time.h"

class ImportColMapConf;

class ImportDoc : public CsvDoc
{
public:
    DECLARE_TM(ImportDoc)

    ImportDoc();
    virtual ~ImportDoc();

    date_t GetRecordDate(const record_t *record) const;
    timo_t GetRecordTime(const record_t *record) const;

    void SetColMap(ImportColMapConf *colMap)
    {
        m_colMap = colMap;
    }

    ImportColMapConf *GetColMap()
    {
        return m_colMap;
    }

    wxString GetCsvTitle(int i) const;
    int GetDataField(int i) const;
    wxString GetDataFieldName(int i) const;
    bool SetDataFieldByName(int i, const wxString &name);
    int GetCsvCol(int i) const;

    struct ImportingRows {
        int importRow;
        int year;
        date_t date;
        timo_t time;
    };

private:
    ImportColMapConf *m_colMap;

    wxArrayString m_colTitles;
    enum column_type *m_types;
    int *m_dataFields;
    int *m_csvCols;

    int Reading(std::istream &is) override;
    int Writing(std::ostream &os) override;
};

#endif /* _HA_IMPORT_IMPORT_DOC_H_ */
