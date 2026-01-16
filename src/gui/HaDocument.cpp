#include <wx/log.h>
#include <wx/msgdlg.h>
#include <wx/textdlg.h>

#include "HaDocument.h"

#include "ChangePassDialog.h"
#include "HaDefs.h"
#include "HaView.h"

#include "file/Cache.h"
#include "file/Exceptions.h"
#include "file/Sqlite3Store.h"

IMPLEMENT_DYNAMIC_CLASS(HaDocument, wxDocument)
IMPLEMENT_TM(HaDocument)

BEGIN_EVENT_TABLE(HaDocument, wxDocument)
EVT_UPDATE_UI(ID_CHANGE_PASS, HaDocument::OnUpdateChangePass)
EVT_MENU(ID_CHANGE_PASS, HaDocument::OnChangePass)
END_EVENT_TABLE()

const char *const HaDocument::IV = APP_NAME;

HaDocument::HaDocument() : wxDocument(), m_doc(nullptr), m_pass()
{
    wxLog::AddTraceMask(TM);
}

HaDocument::~HaDocument()
{
}

bool HaDocument::OnNewDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    m_doc = new Cache();
    // updating must be emitted explicitly here
    UpdateAllViews();
    return wxDocument::OnNewDocument();
}

bool HaDocument::OnCloseDocument()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    delete m_doc;
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
            m_doc = new Cache(new Sqlite3Store(w2s(fileName), w2s(m_pass), IV));
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
    auto *view = GetView();
    if (view != nullptr) {
        view->SaveContents();
    }
    auto *store = new Sqlite3Store(w2s(fileName), w2s(m_pass), IV);
    m_doc->saveAs(store);
    return true;
}

void HaDocument::Modify(bool modified)
{
    wxLogTrace(TM, "\"%s(%i)\" called.", __WXFUNCTION__, modified);
    wxDocument::Modify(modified);
}

const std::string &HaDocument::GetSection(const std::string &name) const
{
    return m_doc->get(name);
}

const std::string &HaDocument::GetOrCreateSection(const std::string &name)
{
    try {
        return GetSection(name);
    } catch ([[maybe_unused]] SectionNotFound &e) {
        SaveSection(name, "");
        return GetSection(name);
    }
}

void HaDocument::SaveSection(const std::string &name, const std::string &content)
{
    m_doc->put(name, content);
}

void HaDocument::DeleteSection(const std::string &name)
{
    m_doc->remove(name);
}

void HaDocument::SaveOrDeleteSection(const std::string &name, const std::string &content)
{
    if (!content.empty()) {
        SaveSection(name, content);
    } else {
        DeleteSection(name);
    }
}

void HaDocument::DeleteSectionPrefix(const std::string &prefix)
{
    m_doc->removePrefix(prefix);
}

void HaDocument::DeleteAllSections()
{
    m_doc->removeAll();
}

void HaDocument::ForEachSection(std::function<bool(const std::string &)> callback) const
{
    m_doc->forEach(callback);
}

void HaDocument::OnChange(wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    Modify(true);
    // skip to allow the original operation
    event.Skip();
}

void HaDocument::OnUpdateChangePass(wxUpdateUIEvent &event)
{
    event.Enable(true);
}

void HaDocument::OnChangePass([[maybe_unused]] wxCommandEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    ChangePassDialog dlg(nullptr, m_pass);
    if (dlg.ShowModal() == wxID_OK) {
        auto pass = dlg.GetPass();
        CryptoStore *store = dynamic_cast<CryptoStore *>(m_doc->getStore());
        if (store != nullptr) {
            store->changePass(w2s(pass));
        }
        m_pass = pass;
    }
}

HaView *HaDocument::GetView() const
{
    return dynamic_cast<HaView *>(this->GetFirstView());
}
