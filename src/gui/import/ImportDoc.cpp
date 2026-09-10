#include <wx/log.h>
#include <wx/translation.h>

#include "ImportDoc.h"

ImportDoc::ImportDoc() : HaData()
{
    wxLog::AddTraceMask(TM);
}

ImportDoc::~ImportDoc()
{
}
