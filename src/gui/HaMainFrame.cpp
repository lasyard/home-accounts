#include <wx/aboutdlg.h>
#include <wx/config.h>
#include <wx/notebook.h>
#include <wx/stdpaths.h>
#include <wx/utils.h>
#include <wx/wx.h>
#include <wx/xrc/xmlres.h>

#include "HaMainFrame.h"

#include "HaApp.h"
#include "HaDefs.h"
#include "Utils.h"

IMPLEMENT_DYNAMIC_CLASS(HaMainFrame, wxDocParentFrame)
IMPLEMENT_TM(HaMainFrame)

// Event propagating order: doc -> view -> frame.
BEGIN_EVENT_TABLE(HaMainFrame, wxDocParentFrame)
EVT_CLOSE(HaMainFrame::OnClose)
EVT_UPDATE_UI(ID_CHANGE_PASS, HaMainFrame::OnUpdateMenu)
EVT_UPDATE_UI(ID_INSERT, HaMainFrame::OnUpdateMenu)
EVT_UPDATE_UI(wxID_DELETE, HaMainFrame::OnUpdateMenu)
EVT_UPDATE_UI(ID_IMPORT, HaMainFrame::OnUpdateMenu)
// Don't know why this ID cannot be wxID_ABOUT.
EVT_MENU(ID_ABOUT, HaMainFrame::OnAbout)
EVT_MENU(ID_LICENSE, HaMainFrame::OnLicense)
EVT_MENU(ID_WX_INFO, HaMainFrame::OnWxInfo)
END_EVENT_TABLE()

const wxString HaMainFrame::CFG_FILE_HISTORY = "FileHistory";
const wxString HaMainFrame::LICENSE_FILE_NAME = "LICENSE";

HaMainFrame::HaMainFrame() : wxDocParentFrame()
{
    wxLog::AddTraceMask(TM);
}

HaMainFrame::HaMainFrame(
    wxDocManager *manager,
    wxFrame *parent,
    wxWindowID id,
    const wxString &title,
    const wxPoint &pos,
    const wxSize &size,
    long type,
    const wxString &name
)
    : wxDocParentFrame(manager, parent, id, title, pos, size, type, name)
{
    wxLog::AddTraceMask(TM);
#ifdef __WXMSW__
    wxIconBundle ib;
    ib.AddIcon(wxICON(ICON16));
    ib.AddIcon(wxICON(ICON32));
    SetIcons(ib);
#endif
    auto *res = wxXmlResource::Get();
    auto *menuBar = res->LoadMenuBar("menuBar");
    SetMenuBar(menuBar);
    auto *statusBar = new wxStatusBar(this, wxID_ANY, wxST_SIZEGRIP);
    statusBar->SetFieldsCount();
    SetStatusBar(statusBar);
    auto *sizer = new wxBoxSizer(wxVERTICAL);
    m_book = new wxNotebook(this, ID_BOOK);
    sizer->Add(m_book, wxSizerFlags().Expand().Border(wxALL, 0).Proportion(1));
    SetSizer(sizer);
    m_book->Show(false);
    SetSize(1080, 720);
    // the config file is in `~/Library/Preferences` on macOS
    wxConfig config(_(APP_NAME));
    config.SetPath(CFG_FILE_HISTORY);
    m_docManager->FileHistoryLoad(config);
    auto *fileMenu = GetMenuBar()->GetMenu(GetMenuBar()->FindMenu(_("File")));
    m_docManager->FileHistoryUseMenu(fileMenu);
    m_docManager->FileHistoryAddFilesToMenu();
}

HaMainFrame::~HaMainFrame()
{
}

void HaMainFrame::OnClose([[maybe_unused]] wxCloseEvent &event)
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    wxConfig config(_(APP_NAME));
    config.SetPath(CFG_FILE_HISTORY);
    m_docManager->FileHistorySave(config);
    Destroy();
}

void HaMainFrame::OnAbout([[maybe_unused]] wxCommandEvent &event)
{
    const wxString &appName = wxGetApp().GetAppDisplayName();
    wxAboutDialogInfo info;
    info.SetName(appName);
    info.SetVersion(APP_VERSION);
    wxString date = __DATE__;
    info.SetCopyright("(C) " + date.Right(4) + " " AUTHOR " " EMAIL);
    info.AddDeveloper(AUTHOR " " EMAIL);
    wxAboutBox(info);
}

void HaMainFrame::OnLicense([[maybe_unused]] wxCommandEvent &event)
{
    wxString text;
    wxString resDir = wxStandardPaths::Get().GetResourcesDir();
    Utils::ReadAllText(text, wxFileName(resDir, LICENSE_FILE_NAME).GetFullPath());
#if wxUSE_UNICODE
    const wxString copyrightSign = s2w("\xc2\xa9");
    text.Replace("(c)", copyrightSign);
    text.Replace("(C)", copyrightSign);
#endif
    Utils::ShowTextBox(_("License"), text);
}

void HaMainFrame::OnWxInfo([[maybe_unused]] wxCommandEvent &event)
{
    wxInfoMessageBox(this);
}

void HaMainFrame::OnUpdateMenu(wxUpdateUIEvent &event)
{
    // there is no doc/view if the event propagated here
    event.Enable(false);
}

void HaMainFrame::OnUpdateCheckMenu(wxUpdateUIEvent &event)
{
    event.Enable(false);
    event.Check(false);
}
