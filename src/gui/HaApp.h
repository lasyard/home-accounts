#ifndef _GUI_HA_APP_H_
#define _GUI_HA_APP_H_

#include <wx/app.h>
#include <wx/docview.h>

#include "Common.h"

class HaApp : public wxApp
{
public:
    DECLARE_TM()

    bool OnInit() override;
    int OnExit() override;

private:
    wxDocManager *m_docManager;
    // This must be long lived.
    wxLocale m_locale;
};

DECLARE_APP(HaApp)

#endif /* _GUI_HA_APP_H_ */
