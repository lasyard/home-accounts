#ifndef _GUI_COMMON_H_
#define _GUI_COMMON_H_

#include <wx/log.h>
#include <wx/xrc/xmlres.h>

#define DECLARE_TM()      static const wxChar *const TM;
#define IMPLEMENT_TM(cls) const wxChar *const cls::TM = _(#cls);

#endif /* _GUI_COMMON_H_ */
