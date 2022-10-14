#ifndef _HA_HA_DOCUMENT_H_
#define _HA_HA_DOCUMENT_H_

#include "../HaDocumentBase.h"
#include "data/DataFile.h"

class SectionFile;

class HaDocument : public HaDocumentBase
{
    DECLARE_DYNAMIC_CLASS(HaDocument)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    HaDocument() : HaDocumentBase(), m_dataFile()
    {
        wxLog::AddTraceMask(TM);
    }

    virtual ~HaDocument()
    {
    }

    DataFile &getDataFile()
    {
        return m_dataFile;
    }

    void loadData(const wxString &name);

private:
    DataFile m_dataFile;
};

#endif /* _HA_HA_DOCUMENT_H_ */
