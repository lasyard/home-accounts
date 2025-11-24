#ifndef _HA_GUI_HA_ART_PROVIDER_H_
#define _HA_GUI_HA_ART_PROVIDER_H_

#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/log.h>

class HaArtProvider : public wxArtProvider
{
public:
    HaArtProvider(const wxString &dir) : wxArtProvider(), m_dir(dir)
    {
    }

protected:
    wxString m_dir;

    wxBitmap CreateBitmap(const wxArtID &id, const wxArtClient &client, const wxSize &size) override;
};

#endif /* _HA_GUI_HA_ART_PROVIDER_H_ */
