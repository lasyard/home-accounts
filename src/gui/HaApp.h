#ifndef _HA_GUI_HA_APP_H_
#define _HA_GUI_HA_APP_H_

#include <wx/app.h>
#include <wx/docview.h>

#include "Utils.h"

class HaApp : public wxApp
{
public:
    DECLARE_TM()

    bool OnInit() override;
    int OnExit() override;

private:
    wxDocManager *m_docManager;

    // this must be long lived
    wxLocale m_locale;
};

DECLARE_APP(HaApp)

#endif /* _HA_GUI_HA_APP_H_ */
