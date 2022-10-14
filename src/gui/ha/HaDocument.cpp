#include "HaDocument.h"

IMPLEMENT_DYNAMIC_CLASS(HaDocument, HaDocumentBase)
IMPLEMENT_TM(HaDocument)

BEGIN_EVENT_TABLE(HaDocument, HaDocumentBase)
END_EVENT_TABLE()

void HaDocument::loadData(const wxString &name)
{
    wxString content;
    try {
        GetSection(name, content);
        m_dataFile.read(content.ToStdString());
    } catch (std::exception &e) {
        wxLogError(e.what());
    }
}
