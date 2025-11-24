#include <wx/fs_arc.h>
#include <wx/stdpaths.h>
#include <wx/xrc/xmlres.h>

#include "HaApp.h"

#include "HaArtProvider.h"
#include "HaDocument.h"
#include "HaMainFrame.h"
#include "HaView.h"

IMPLEMENT_APP(HaApp)
IMPLEMENT_TM(HaApp)

bool HaApp::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }
    wxLog::AddTraceMask(TM);
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    m_locale.Init();
    if (!m_locale.AddCatalog(APP_NAME) || !m_locale.AddCatalog("xrc")) {
        wxLogError("Could not load translation files for \"" + m_locale.GetCanonicalName() + "\" locale.");
    }
    m_locale.AddCatalog("wxstd");
    wxFileSystem::AddHandler(new wxArchiveFSHandler());
    wxXmlResource::Get()->InitAllHandlers();
    wxString resDir = wxStandardPaths::Get().GetResourcesDir();
    wxXmlResource::Get()->LoadFile(wxFileName(resDir, XRS_FILE));
    wxImage::AddHandler(new wxPNGHandler());
    SetVendorName("Lasy");
    SetAppName(_(APP_NAME));
    SetAppDisplayName(_(APP_NAME));
    wxArtProvider::Push(new HaArtProvider(resDir));
    m_docManager = new wxDocManager();
    (void)new wxDocTemplate(
        m_docManager,
        GetAppDisplayName(),
        "*.ha",
        "",
        "ha",
        GetAppDisplayName() + _(" Doc"),
        GetAppDisplayName() + _(" View"),
        CLASSINFO(HaDocument),
        CLASSINFO(HaView)
    );
    m_docManager->SetMaxDocsOpen(1);
    wxFrame *frame = new HaMainFrame(m_docManager, nullptr, wxID_ANY, GetAppDisplayName());
    frame->Center(wxBOTH);
    frame->Show();
    SetTopWindow(frame);
    return true;
}

int HaApp::OnExit()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    delete m_docManager;
    return wxApp::OnExit();
}
