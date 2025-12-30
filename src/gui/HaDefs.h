#ifndef _HA_GUI_HA_DEFS_H_
#define _HA_GUI_HA_DEFS_H_

#include <wx/xrc/xmlres.h>

// menus
#define ID_CHANGE_PASS XRCID("changePass")
#define ID_INSERT      XRCID("insert")
#define ID_ABOUT       XRCID("about")
#define ID_LICENSE     XRCID("license")
#define ID_WX_INFO     XRCID("wxInfo")

// controls
#define ID_BOOK (wxID_HIGHEST + 1)
#define ID_DATE (wxID_HIGHEST + 2)

// arts
constexpr const char *LASYARD_LOGO = "lasyard_logo.png";
constexpr const char *APP_ICON = "app_icon.png";

#endif /* _HA_GUI_HA_DEFS_H_ */
