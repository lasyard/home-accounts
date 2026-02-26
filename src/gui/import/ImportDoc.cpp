#include <wx/log.h>
#include <wx/translation.h>

#include "ImportDoc.h"

IMPLEMENT_TM(ImportDoc)

ImportDoc::ImportDoc()
{
    wxLog::AddTraceMask(TM);
}

ImportDoc::~ImportDoc()
{
}

bool ImportDoc::ReadStream([[maybe_unused]] std::istream &is)
{
    return true;
}
