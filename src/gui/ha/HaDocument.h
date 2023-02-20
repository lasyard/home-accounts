#ifndef _HA_HA_DOCUMENT_H_
#define _HA_HA_DOCUMENT_H_

#include "../HaDocumentBase.h"
#include "data/ConfigPodsTraits.h"
#include "data/CsvVecDao.h"
#include "data/DataDao.h"

class SectionFile;

class HaDocument : public HaDocumentBase
{
    DECLARE_DYNAMIC_CLASS(HaDocument)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    HaDocument() : HaDocumentBase(), m_dataDao(), m_accountsDao()
    {
        wxLog::AddTraceMask(TM);
    }

    virtual ~HaDocument()
    {
    }

    DataDao &GetDataDao()
    {
        return m_dataDao;
    }

    CsvVecDao<struct account> &GetAccountsDao()
    {
        return m_accountsDao;
    }

    void LoadData(const wxString &name);
    void SaveData(const wxString &name);

    void LoadConfigs()
    {
        std::string content;
        GetOrCreateSection("configs/accounts", content);
        m_dataDao.readString(content);
    }

private:
    DataDao m_dataDao;
    CsvVecDao<struct account> m_accountsDao;

    void GetOrCreateSection(const std::string &name, std::string &content);
};

#endif /* _HA_HA_DOCUMENT_H_ */
