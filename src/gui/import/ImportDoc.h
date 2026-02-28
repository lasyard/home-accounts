#ifndef _HA_IMPORT_IMPORT_DOC_H_
#define _HA_IMPORT_IMPORT_DOC_H_

#include "../CsvDoc.h"

class ImportDoc : public CsvDoc
{
public:
    DECLARE_TM()

    ImportDoc();
    virtual ~ImportDoc();

    bool ReadStream(std::istream &is) override;

private:
    enum column_type *m_types;
};

#endif /* _HA_IMPORT_IMPORT_DOC_H_ */
