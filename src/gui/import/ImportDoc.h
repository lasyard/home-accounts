#ifndef _HA_IMPORT_IMPORT_DOC_H_
#define _HA_IMPORT_IMPORT_DOC_H_

#include <memory>
#include <string>

#include <wx/arrstr.h>

#include "../CsvDoc.h"

class ImportColMapConf;

class ImportDoc : public CsvDoc
{
public:
    DECLARE_TM(ImportDoc)

    ImportDoc();
    virtual ~ImportDoc();

    void SetColumnMap(const std::string &str);

    wxString GetCsvTitle(int i) const;
    wxString GetDataFieldName(int i) const;
    int GetDataField(int i) const;

private:
    std::unique_ptr<ImportColMapConf> m_colMap;
    wxArrayString m_colTitles;
    enum column_type *m_types;
    int *m_dataFields;

    int Reading(std::istream &is) override;
    int Writing(std::ostream &os) override;
};

#endif /* _HA_IMPORT_IMPORT_DOC_H_ */
