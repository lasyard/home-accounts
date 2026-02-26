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
};

#endif /* _HA_IMPORT_IMPORT_DOC_H_ */
