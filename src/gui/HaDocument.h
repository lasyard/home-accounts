#ifndef _GUI_HA_DOCUMENT_H_
#define _GUI_HA_DOCUMENT_H_

#include <wx/datetime.h>
#include <wx/docview.h>

#include "Common.h"

#include "data/AccountsDao.h"
#include "data/ConfigPodsTraits.h"
#include "data/CsvIdVecDao.h"
#include "data/DataDao.h"

#include "file/FileExeptions.h"

class HaView;
class SectionFile;

class HaDocument : public wxDocument
{
    DECLARE_DYNAMIC_CLASS(HaDocument)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    HaDocument();
    virtual ~HaDocument();

    bool OnNewDocument() override;
    bool OnCloseDocument() override;
    bool DeleteContents() override;
    bool DoOpenDocument(const wxString &fileName) override;
    bool DoSaveDocument(const wxString &fileName) override;

    void Modify(bool modified) override;

    void GetSectionNames(wxArrayString &names) const;
    void GetSection(const wxString &name, wxString &content) const;
    void SaveSection(const wxString &name, const wxString &content);
    void DeleteSection(const wxString &name);

    void OnChange(wxCommandEvent &event);

    void OnUpdateChangePass(wxUpdateUIEvent &event);
    void OnChangePass(wxCommandEvent &event);

    void TryLoad(DaoBase &dao);
    void DoSave(DaoBase &dao);
    void TryLoadData(const wxDateTime &date);
    void TryLoadBill(int batch);

    /**
     * @brief Create a new bill section.
     *
     * @param title the title of bill
     * @param content the string to parse
     * @param account the account name
     * @param channel the channel name
     * @return true created successfully
     * @return false failed to create
     */
    [[nodiscard]] bool CreateBill(const wxString &title, const wxString &content, int account);

    DataDao &GetDataDao()
    {
        return m_dataDao;
    }

    DataDao &GetBillDao()
    {
        return m_billDao;
    }

    CsvIdVecDao<struct owner> &GetOwnersDao()
    {
        return m_ownersDao;
    }

    CsvIdVecDao<struct account_type> &GetAccountTypesDao()
    {
        return m_accountTypesDao;
    }

    AccountsDao &GetAccountsDao()
    {
        return m_accountsDao;
    }

    CsvIdVecDao<struct batch> &GetBatchesDao()
    {
        return m_batchesDao;
    }

private:
    static const char *const IV;

    static const std::string OWNERS_SECTION_NAME;
    static const std::string ACCOUNT_TYPES_SECTION_NAME;
    static const std::string ACCOUNTS_SECTION_NAME;
    static const std::string BATCHES_SECTION_NAME;

    static const std::string DATA_SECTION_PREFIX;
    static const std::string BILL_SECTION_PREFIX;

    SectionFile *m_doc;
    wxString m_pass;

    DataDao m_dataDao;
    DataDao m_billDao;
    CsvIdVecDao<struct owner> m_ownersDao;
    CsvIdVecDao<struct account_type> m_accountTypesDao;
    AccountsDao m_accountsDao;
    CsvIdVecDao<struct batch> m_batchesDao;

    static std::string DataSectionName(const wxDateTime &date)
    {
        return DATA_SECTION_PREFIX + s(wxString::Format("/%04d/%02d", date.GetYear(), date.GetMonth() + 1));
    }

    static std::string BillSectionName(int batch)
    {
        return BILL_SECTION_PREFIX + s(wxString::Format("/%04d", batch));
    }

    HaView *GetView() const;
};

#endif /* _GUI_HA_DOCUMENT_H_ */
