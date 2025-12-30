#include <wx/filename.h>

#include "HaArtProvider.h"

wxBitmap HaArtProvider::CreateBitmap(const wxArtID &id, const wxArtClient &client, const wxSize &size)
{
    try {
        return wxBitmap(wxFileName(m_dir, id).GetFullPath(), wxBITMAP_TYPE_PNG);
    } catch (...) {
        return wxArtProvider::CreateBitmap(id, client, size);
    }
}
