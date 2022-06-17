#ifndef _RAW_RAW_APP_H_
#define _RAW_RAW_APP_H_

#include <wx/app.h>
#include <wx/docview.h>

#include "../Common.h"

class RawApp : public wxApp
{
public:
    DECLARE_TM()

    bool OnInit() override;
    int OnExit() override;

private:
    wxDocManager *m_docManager;
};

DECLARE_APP(RawApp)

#endif /* _RAW_RAW_APP_H_ */
