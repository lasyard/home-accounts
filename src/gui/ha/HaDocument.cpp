#include <sstream>

#include "HaDocument.h"

IMPLEMENT_DYNAMIC_CLASS(HaDocument, HaDocumentBase)
IMPLEMENT_TM(HaDocument)

BEGIN_EVENT_TABLE(HaDocument, HaDocumentBase)
END_EVENT_TABLE()

void HaDocument::LoadData(const wxString &name)
{
    std::string content;
    try {
        GetSection(name, content);
        m_dataFile.read(content);
    } catch (std::exception &e) {
        wxLogError(e.what());
    }
}

void HaDocument::SaveData(const wxString &name)
{
    std::ostringstream os;
    m_dataFile.write(os);
    SaveSection(name, os.str());
}
