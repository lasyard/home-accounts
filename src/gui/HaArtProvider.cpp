#include <wx/filename.h>

#include "HaArtProvider.h"

wxBitmap HaArtProvider::CreateBitmap(const wxArtID &id, const wxArtClient &client, const wxSize &size)
{
    if (id == "logo") {
        wxString fileName = id + ".png";
        return wxBitmap(wxFileName(m_dir, fileName).GetFullPath(), wxBITMAP_TYPE_PNG);
    }
    return wxArtProvider::CreateBitmap(id, client, size);
}
