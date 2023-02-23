#ifndef _GUI_HA_DOCUMENT_H_
#define _GUI_HA_DOCUMENT_H_

#include <wx/docview.h>

#include "Common.h"
#include "HaView.h"
#include "data/ConfigPodsTraits.h"
#include "data/CsvVecDao.h"
#include "data/DataDao.h"

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

    CsvVecDao<struct account> &GetAccountsDao()
    {
        return m_accountsDao;
    }

    void LoadData(const wxString &name);
    void SaveData(const wxString &name);

private:
    static const char *const IV;

    SectionFile *m_doc;
    wxString m_pass;

    DataDao m_dataDao;
    CsvVecDao<struct account> m_accountsDao;

    HaView *GetView() const
    {
        auto view = this->GetFirstView();
        return view != nullptr ? static_cast<HaView *>(view) : nullptr;
    }
};

#endif /* _GUI_HA_DOCUMENT_H_ */
