#ifndef _GUI_HA_ART_PROVIDER_H_
#define _GUI_HA_ART_PROVIDER_H_

#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/log.h>

#include "Common.h"

class HaArtProvider : public wxArtProvider
{
    DECLARE_TM()

public:
    HaArtProvider(const wxString &dir) : wxArtProvider(), m_dir(dir)
    {
        wxLog::AddTraceMask(TM);
    }

protected:
    wxString m_dir;

    wxBitmap CreateBitmap(const wxArtID &id, const wxArtClient &client, const wxSize &size) override;
};

#endif /* _GUI_HA_ART_PROVIDER_H_ */
