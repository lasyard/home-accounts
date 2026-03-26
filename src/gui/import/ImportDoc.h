#ifndef _HA_IMPORT_IMPORT_DOC_H_
#define _HA_IMPORT_IMPORT_DOC_H_

#include <wx/arrstr.h>

#include "../CsvDoc.h"

class ImportColMapConf;

class ImportDoc : public CsvDoc
{
public:
    DECLARE_TM(ImportDoc)

    ImportDoc();
    virtual ~ImportDoc();

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

private:
    ImportColMapConf *m_colMap;

    wxArrayString m_colTitles;
    enum column_type *m_types;
    int *m_dataFields;

    int Reading(std::istream &is) override;
    int Writing(std::ostream &os) override;
};

#endif /* _HA_IMPORT_IMPORT_DOC_H_ */
