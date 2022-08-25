#include <wx/filename.h>

#include "HaArtProvider.h"

IMPLEMENT_TM(HaArtProvider)

wxBitmap HaArtProvider::CreateBitmap(
    const wxArtID &id,
    const wxArtClient &client,
    const wxSize &size
)
{
    wxLogTrace(TM, "\"%s\" called to create image \"%s\".", __WXFUNCTION__, id);
    if (id == "logo") {
        wxString fileName = id + ".png";
        return wxBitmap(wxFileName(m_dir, fileName).GetFullPath(), wxBITMAP_TYPE_PNG);
    }
    return wxArtProvider::CreateBitmap(id, client, size);
}
