#ifndef _GUI_HA_DOCUMENT_H_
#define _GUI_HA_DOCUMENT_H_

#include <wx/docview.h>

#include "Common.h"
#include "HaView.h"
#include "data/AccountsDao.h"
#include "data/ConfigPodsTraits.h"
#include "data/CsvIdVecDao.h"
#include "data/DataDao.h"
#include "file/FileExeptions.h"

class SectionFile;

class HaDocument : public wxDocument
{
    DECLARE_DYNAMIC_CLASS(HaDocument)
    DECLARE_EVENT_TABLE()

public:
    DECLARE_TM()

    HaDocument();
    virtual ~HaDocument();

    template <typename I, int COL> static void GetStringsByCol(const CsvVecDao<I> &dao, wxArrayString &strs)
    {
        strs.Clear();
        for (const auto &i : dao.getData()) {
            strs.push_back(*(const char *const *)CsvRowTraits<I>::getPtr(const_cast<I *>(&i), COL));
        }
    }

    bool OnNewDocument() override;
    bool OnCloseDocument() override;
    bool DeleteContents() override;
    bool DoOpenDocument(const wxString &fileName) override;
    bool DoSaveDocument(const wxString &fileName) override;

    void Modify(bool modified) override;

    void GetSectionNames(wxArrayString &names) const;
    void GetSection(const wxString &name, wxString &content) const;
    void GetSection(const wxString &name, std::string &content) const;
    void SaveSection(const wxString &name, const wxString &content);
    void SaveSection(const wxString &name, const std::string &content);
    void DeleteSection(const wxString &name);

    void OnChange(wxCommandEvent &event);

    void OnUpdateChangePass(wxUpdateUIEvent &event);
    void OnChangePass(wxCommandEvent &event);

    DataDao &GetDataDao()
    {
        return m_dataDao;
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

    CsvIdVecDao<struct channel> &GetChannelsDao()
    {
        return m_channelsDao;
    }

    void TryLoad(const wxString &name, DaoBase &dao)
    {
        try {
            std::string content;
            GetSection(name, content);
            dao.readString(content);
        } catch (SectionNotFound &e) {
            wxLogStatus(e.what());
        } catch (std::exception &e) {
            wxLogError(e.what());
        }
    }

    void DoSave(const wxString &name, DaoBase &dao)
    {
        std::ostringstream os;
        dao.write(os);
        SaveSection(name, os.str());
    }

    void DoSaveData(const wxString &name)
    {
        DoSave(name, m_dataDao);
    }

private:
    static const char *const IV;

    SectionFile *m_doc;
    wxString m_pass;

    DataDao m_dataDao;
    CsvIdVecDao<struct owner> m_ownersDao;
    CsvIdVecDao<struct account_type> m_accountTypesDao;
    AccountsDao m_accountsDao;
    CsvIdVecDao<struct channel> m_channelsDao;

    HaView *GetView() const
    {
        auto view = this->GetFirstView();
        return view != nullptr ? static_cast<HaView *>(view) : nullptr;
    }
};

#endif /* _GUI_HA_DOCUMENT_H_ */
