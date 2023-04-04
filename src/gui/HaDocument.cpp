#include <sstream>

#include <wx/msgdlg.h>
#include <wx/textdlg.h>

#include "Configs.h"
#include "HaDocument.h"
#include "file/SectionFile.h"
#include "file/sqlite3/Sqlite3File.h"

IMPLEMENT_DYNAMIC_CLASS(HaDocument, wxDocument)
IMPLEMENT_TM(HaDocument)

BEGIN_EVENT_TABLE(HaDocument, wxDocument)
END_EVENT_TABLE()

const char *const HaDocument::IV = "HomeAccounts";

HaDocument::HaDocument()
    : wxDocument(),
      m_doc(new SectionFile()),
      m_pass(),
      m_dataDao(),
      m_ownersDao(),
      m_accountTypesDao(),
      m_accountsDao(),
      m_channelsDao()
{
    wxLog::AddTraceMask(TM);
}

HaDocument::~HaDocument()
{
    delete m_doc;
}

bool HaDocument::OnNewDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    // Updating must be emitted explicitly here.
    UpdateAllViews();
    return wxDocument::OnNewDocument();
}

bool HaDocument::OnCloseDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    return wxDocument::OnCloseDocument();
}

// Called twice when closing, one in doc close, one in view close.
bool HaDocument::DeleteContents()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    return wxDocument::DeleteContents();
}

bool HaDocument::DoOpenDocument(const wxString &fileName)
{
    wxLogTrace(TM, "\"%s(%s)\" called.", __WXFUNCTION__, fileName);
    wxPasswordEntryDialog dlgPass(nullptr, _("Input the password for the file:"));
    if (dlgPass.ShowModal() == wxID_OK) {
        m_pass = dlgPass.GetValue();
        try {
            auto store = new Sqlite3File(fileName.ToStdString(), m_pass.ToStdString(), IV);
            m_doc->attach(store);
            TryLoad(Configs::OWNERS_SECTION_NAME, m_ownersDao);
            TryLoad(Configs::ACCOUNT_TYPES_SECTION_NAME, m_accountTypesDao);
            TryLoad(Configs::ACCOUNTS_SECTION_NAME, m_accountsDao);
            m_accountsDao.setOwerJoint(m_ownersDao.getJoint<1, 0>());
            m_accountsDao.setTypeJoint(m_accountTypesDao.getJoint<1, 0>());
            TryLoad(Configs::CHANNELS_SECTION_NAME, m_channelsDao);
            m_dataDao.setAccountJoint(m_accountsDao.getJoint<1, 0>());
            m_dataDao.setChannelJoint(m_channelsDao.getJoint<1, 0>());
            return true;
        } catch (std::runtime_error &e) {
            wxLogError("Failed to open \"%s\": %s", (const char *)fileName, e.what());
        }
    }
    return false;
}

bool HaDocument::DoSaveDocument(const wxString &fileName)
{
    wxLogTrace(TM, "\"%s(%s)\" called.", __WXFUNCTION__, fileName);
    auto view = GetView();
    if (view != nullptr) {
        view->SaveContents();
    }
    auto store = new Sqlite3File(fileName.ToStdString(), m_pass.ToStdString(), IV);
    m_doc->saveAs(store);
    return true;
}

void HaDocument::Modify(bool modified)
{
    wxLogTrace(TM, "\"%s(%i)\" called.", __WXFUNCTION__, modified);
    wxDocument::Modify(modified);
}

void HaDocument::GetSectionNames(wxArrayString &names) const
{
    std::vector<std::string> stdNames;
    m_doc->getSectionNames(stdNames);
    for (auto const &name : stdNames) {
        names.push_back(name);
    }
}

void HaDocument::GetSection(const wxString &name, wxString &content) const
{
    content = m_doc->get(name.ToStdString());
}

void HaDocument::GetSection(const wxString &name, std::string &content) const
{
    content = m_doc->get(name.ToStdString());
}

void HaDocument::SaveSection(const wxString &name, const wxString &content)
{
    SaveSection(name, content.ToStdString());
}

void HaDocument::SaveSection(const wxString &name, const std::string &content)
{
    m_doc->put(name.ToStdString(), content);
}

void HaDocument::DeleteSection(const wxString &name)
{
    m_doc->remove(name.ToStdString());
}

void HaDocument::OnChange(wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    Modify(true);
    event.Skip();
}

void HaDocument::ChangePass(const wxString &pass)
{
    CryptedSectionStore *store = dynamic_cast<CryptedSectionStore *>(m_doc->getStore());
    if (store != nullptr) {
        store->changePass(pass.ToStdString());
    }
    m_pass = pass;
}
