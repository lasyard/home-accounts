#ifndef _HA_HA_DOCUMENT_H_
#define _HA_HA_DOCUMENT_H_

#include "../HaDocumentBase.h"

class SectionFile;

class HaDocument : public HaDocumentBase
{
    DECLARE_DYNAMIC_CLASS(HaDocument)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    HaDocument() : HaDocumentBase()
    {
        wxLog::AddTraceMask(TM);
    }

    virtual ~HaDocument()
    {
    }
};

#endif /* _HA_HA_DOCUMENT_H_ */
