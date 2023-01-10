#include <stdexcept>

#include <wx/textdlg.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "HaDocumentBase.h"
#include "HaViewBase.h"
#include "file/SectionFile.h"
#include "file/sqlite3/Sqlite3File.h"

IMPLEMENT_DYNAMIC_CLASS(HaDocumentBase, wxDocument)
IMPLEMENT_TM(HaDocumentBase)

BEGIN_EVENT_TABLE(HaDocumentBase, wxDocument)
END_EVENT_TABLE()

const char *const HaDocumentBase::IV = "HomeAccount";

bool HaDocumentBase::OnNewDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    if (m_doc != nullptr) {
        delete m_doc;
    }
    m_doc = new SectionFile();
    return wxDocument::OnNewDocument();
}

bool HaDocumentBase::OnCloseDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    return wxDocument::OnCloseDocument();
}

// Called twice when closing, one in doc close, one in view close.
bool HaDocumentBase::DeleteContents()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    if (m_doc != nullptr) {
        delete m_doc;
        m_doc = nullptr;
    }
    return true;
}

bool HaDocumentBase::DoOpenDocument(const wxString &fileName)
{
    wxLogTrace(TM, "\"%s(%s)\" called.", __WXFUNCTION__, fileName);
    wxPasswordEntryDialog dlgPass(nullptr, _("Input the password for the file:"));
    if (dlgPass.ShowModal() == wxID_OK) {
        m_pass = dlgPass.GetValue();
        try {
            auto store = new Sqlite3File(fileName.ToStdString(), m_pass.ToStdString(), IV);
            m_doc = new SectionFile();
            m_doc->attach(store);
            return true;
        } catch (std::runtime_error &e) {
            wxLogError("Failed to open \"%s\": %s", (const char *)fileName, e.what());
        }
    }
    return false;
}

bool HaDocumentBase::DoSaveDocument(const wxString &fileName)
{
    wxLogTrace(TM, "\"%s(%s)\" called.", __WXFUNCTION__, fileName);
    auto view = GetView<HaViewBase>();
    if (m_doc == nullptr) {
        m_doc = new SectionFile();
    }
    if (view != nullptr) {
        view->SaveContents();
    }
    auto store = new Sqlite3File(fileName.ToStdString(), m_pass.ToStdString(), IV);
    m_doc->saveAs(store);
    return true;
}

void HaDocumentBase::Modify(bool modified)
{
    wxLogTrace(TM, "\"%s(%i)\" called.", __WXFUNCTION__, modified);
    wxDocument::Modify(modified);
    auto view = GetView<HaViewBase>();
    if (view != nullptr && !modified) {
        view->DiscardEdits();
    }
}

void HaDocumentBase::GetSectionNames(wxArrayString &names) const
{
    wxASSERT(m_doc != nullptr);
    std::vector<std::string> stdNames;
    m_doc->getSectionNames(stdNames);
    for (auto const &name : stdNames) {
        names.push_back(name);
    }
}

void HaDocumentBase::GetSection(const wxString &name, wxString &content) const
{
    wxASSERT(m_doc != nullptr);
    content = m_doc->get(name.ToStdString());
}

void HaDocumentBase::GetSection(const wxString &name, std::string &content) const
{
    wxASSERT(m_doc != nullptr);
    content = m_doc->get(name.ToStdString());
}

void HaDocumentBase::SaveSection(const wxString &name, const wxString &content)
{
    SaveSection(name, content.ToStdString());
}

void HaDocumentBase::SaveSection(const wxString &name, const std::string &content)
{
    wxASSERT(m_doc != nullptr);
    m_doc->put(name.ToStdString(), content);
}

void HaDocumentBase::DeleteSection(const wxString &name)
{
    wxASSERT(m_doc != nullptr);
    m_doc->remove(name.ToStdString());
}

void HaDocumentBase::OnChange(wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    Modify(true);
    event.Skip();
}

void HaDocumentBase::ChangePass(const wxString &pass)
{
    if (m_doc != nullptr) {
        CryptedSectionStore *store = dynamic_cast<CryptedSectionStore *>(m_doc->getStore());
        if (store != nullptr) {
            store->changePass(pass.ToStdString());
        }
    }
    m_pass = pass;
}
