#include <sstream>

#include <wx/msgdlg.h>

#include "HaDocument.h"
#include "file/FileExeptions.h"

IMPLEMENT_DYNAMIC_CLASS(HaDocument, HaDocumentBase)
IMPLEMENT_TM(HaDocument)

BEGIN_EVENT_TABLE(HaDocument, HaDocumentBase)
END_EVENT_TABLE()

void HaDocument::LoadData(const wxString &name)
{
    try {
        std::string content;
        GetSection(name, content);
        m_dataDao.readString(content);
    } catch (std::exception &e) {
        wxLogError(e.what());
    }
}

void HaDocument::SaveData(const wxString &name)
{
    std::ostringstream os;
    m_dataDao.write(os);
    SaveSection(name, os.str());
}

void HaDocument::GetOrCreateSection(const std::string &name, std::string &content)
{
    try {
        GetSection(name, content);
    } catch (SectionNotFound &e) {
        wxMessageBox(_("Section \"") + e.getName() + _("\" not found, will be created."));
        SaveSection(name, content);
        Modify(true);
    } catch (std::exception &e) {
        wxLogError(e.what());
    }
}
