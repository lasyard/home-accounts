#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <regex>
#include <vector>

#include <wx/datetime.h>
#include <wx/filename.h>
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
EVT_UPDATE_UI(ID_CHANGE_PASS, HaDocument::OnUpdateMenu)
EVT_MENU(ID_CHANGE_PASS, HaDocument::OnChangePass)
EVT_UPDATE_UI(ID_IMPORT, HaDocument::OnUpdateMenu)
EVT_MENU(ID_IMPORT, HaDocument::OnImport)
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
    CreateBackupIfNeeded(fileName);
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

void HaDocument::OnUpdateMenu(wxUpdateUIEvent &event)
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

void HaDocument::OnImport([[maybe_unused]] wxCommandEvent &event)
{
}

HaView *HaDocument::GetView() const
{
    return dynamic_cast<HaView *>(this->GetFirstView());
}

bool HaDocument::CreateBackupIfNeeded(const wxString &fileName)
{
    auto fn = wxFileName(fileName);
    if (!fn.FileExists()) {
        return false;
    }
    // prepare matching regex for backup files: stem_YYYY-MM-DD_hhmmss(.ext)?
    std::string stem = w2s(fn.GetName());
    std::string ext = w2s(fn.GetExt());
    std::string pattern = "^" + EscapeRegex(stem) + "_[0-9]{4}-[0-9]{2}-[0-9]{2}_[0-9]{6}";
    if (!ext.empty()) {
        pattern += "\\." + EscapeRegex(ext);
    }
    pattern += "$";
    std::regex rx(pattern);
    // collect existing backups in the same directory
    std::vector<std::pair<std::filesystem::path, std::filesystem::file_time_type>> backs;
    std::filesystem::path dir = w2s(fn.GetPath());
    if (std::filesystem::exists(dir) && std::filesystem::is_directory(dir)) {
        for (auto &entry : std::filesystem::directory_iterator(dir)) {
            if (!entry.is_regular_file())
                continue;
            auto name = entry.path().filename().string();
            if (std::regex_match(name, rx)) {
                try {
                    backs.emplace_back(entry.path(), std::filesystem::last_write_time(entry.path()));
                } catch (...) {
                    backs.emplace_back(entry.path(), std::filesystem::file_time_type::min());
                }
            }
        }
    }
    // sort by time ascending (oldest first)
    std::sort(backs.begin(), backs.end(), [](auto &a, auto &b) { return a.second < b.second; });
    // remove oldest until we have space for the new backup (ensure current count < maxBackups)
    while (backs.size() >= MAX_BACKUPS) {
        try {
            std::filesystem::remove(backs.front().first);
        } catch (...) {
            // ignore remove errors
        }
        backs.erase(backs.begin());
    }
    // create timestamped backup filename
    auto now = wxDateTime::Now();
    wxString date = now.FormatISODate();  // YYYY-MM-DD
    wxString time = now.Format("%H%M%S"); // hhmmss
    wxString backupName = fn.GetName() + "_" + date + "_" + time;
    if (!fn.GetExt().IsEmpty()) {
        backupName += "." + fn.GetExt();
    }
    wxFileName backupFn(fn.GetPath(), backupName);
    // perform rename
    if (!wxRenameFile(fn.GetFullPath(), backupFn.GetFullPath())) {
        wxLogError(_("Failed to create backup file \"%s\"."), backupFn.GetFullPath());
        return false;
    }
    return true;
}
