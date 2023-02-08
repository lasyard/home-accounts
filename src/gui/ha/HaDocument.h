#ifndef _HA_HA_DOCUMENT_H_
#define _HA_HA_DOCUMENT_H_

#include "../HaDocumentBase.h"
#include "data/DataDao.h"

class SectionFile;

class HaDocument : public HaDocumentBase
{
    DECLARE_DYNAMIC_CLASS(HaDocument)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    HaDocument() : HaDocumentBase(), m_dataDao()
    {
        wxLog::AddTraceMask(TM);
    }

    virtual ~HaDocument()
    {
    }

    DataDao &GetDataFile()
    {
        return m_dataDao;
    }

    void LoadData(const wxString &name);
    void SaveData(const wxString &name);

private:
    DataDao m_dataDao;
};

#endif /* _HA_HA_DOCUMENT_H_ */
