#ifndef _GUI_HA_DOCUMENT_H_
#define _GUI_HA_DOCUMENT_H_

#include <wx/docview.h>

#include "Common.h"
#include "HaView.h"
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

    static const char *const ACCOUNTS_SECTION_NAME;
    static const char *const CHANNELS_SECTION_NAME;

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
    void GetSection(const wxString &name, std::string &content) const;
    void SaveSection(const wxString &name, const wxString &content);
    void SaveSection(const wxString &name, const std::string &content);
    void DeleteSection(const wxString &name);

    virtual void OnChange(wxCommandEvent &event);

    wxString GetPass() const
    {
        return m_pass;
    }

    void ChangePass(const wxString &pass);

    DataDao &GetDataDao()
    {
        return m_dataDao;
    }

    CsvIdVecDao<struct account> &GetAccountsDao()
    {
        return m_accountsDao;
    }

    CsvIdVecDao<struct channel> &GetChannelsDao()
    {
        return m_channelsDao;
    }

    void GetAccountNames(wxArrayString &names) const
    {
        names.Clear();
        for (const auto &i : m_accountsDao.getData()) {
            names.push_back(i.name);
        }
    }

    void GetChannelNames(wxArrayString &names) const
    {
        names.Clear();
        for (const auto &i : m_channelsDao.getData()) {
            names.push_back(i.name);
        }
    }

    template <typename T> void TryLoad(const wxString &name, Dao<T> &dao)
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

    template <typename T> void DoSave(const wxString &name, Dao<T> &dao)
    {
        std::ostringstream os;
        dao.write(os);
        SaveSection(name, os.str());
    }

    void DoSaveData(const wxString &name)
    {
        DoSave(name, m_dataDao);
    }

    void DoSaveAccounts()
    {
        DoSave(ACCOUNTS_SECTION_NAME, m_accountsDao);
    }

    void DoSaveChannels()
    {
        DoSave(CHANNELS_SECTION_NAME, m_channelsDao);
    }

private:
    static const char *const IV;

    SectionFile *m_doc;
    wxString m_pass;

    DataDao m_dataDao;
    CsvIdVecDao<struct account> m_accountsDao;
    CsvIdVecDao<struct channel> m_channelsDao;

    HaView *GetView() const
    {
        auto view = this->GetFirstView();
        return view != nullptr ? static_cast<HaView *>(view) : nullptr;
    }
};

#endif /* _GUI_HA_DOCUMENT_H_ */
