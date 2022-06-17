#include <wx/fs_arc.h>
#include <wx/xrc/xmlres.h>

#include "RawApp.h"
#include "RawDocument.h"
#include "RawMainFrame.h"
#include "RawView.h"

IMPLEMENT_APP(RawApp)
IMPLEMENT_TM(RawApp)

bool RawApp::OnInit()
{
    if (!wxApp::OnInit()) {
        return false;
    }
    wxLog::AddTraceMask(TM);
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    wxFileSystem::AddHandler(new wxArchiveFSHandler());
    wxXmlResource::Get()->InitAllHandlers();
    wxXmlResource::Get()->LoadFile(wxFileName("res/", XRS_FILE));
    SetVendorName("Lasy");
    SetAppName(_(APP_NAME));
    SetAppDisplayName("Home Accounts");
    m_docManager = new wxDocManager();
    (void)new wxDocTemplate(
        m_docManager,
        GetAppDisplayName(),
        "*.ha",
        "",
        "ha",
        GetAppDisplayName() + _(" Doc"),
        _("Accounts View"),
        CLASSINFO(RawDocument),
        CLASSINFO(RawView)
    );
    m_docManager->SetMaxDocsOpen(1);
    wxFrame *frame = new RawMainFrame(m_docManager, nullptr, wxID_ANY, GetAppDisplayName());
    frame->Center(wxBOTH);
    frame->Show();
    SetTopWindow(frame);
    return true;
}

int RawApp::OnExit()
{
    wxLogTrace(TM, "\"%s\" called.", __WXFUNCTION__);
    delete m_docManager;
    return wxApp::OnExit();
}
