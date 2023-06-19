#ifndef _GUI_DEFS_H_
#define _GUI_DEFS_H_

#include <wx/xrc/xmlres.h>

#define APP_VERSION "1"

// Menus
#define ID_IMPORT      XRCID("import")
#define ID_EXPORT      XRCID("export")
#define ID_CHANGE_PASS XRCID("changePass")
#define ID_INSERT      XRCID("insert")
#define ID_PASTE_BILL  XRCID("pasteBill")
#define ID_RAW_MODE    XRCID("rawMode")
#define ID_ABOUT       XRCID("about")
#define ID_LICENSE     XRCID("license")
#define ID_WX_INFO     XRCID("wxInfo")

// Controls
#define ID_BOOK           XRCID("book")
#define ID_DATE_DATA      XRCID("dateData")
#define ID_BOOK_CONFIGS   XRCID("bookConfigs")
#define ID_CHOICE_ACCOUNT XRCID("choiceAccount")
#define ID_CHOICE_CHANNEL XRCID("choiceChannel")
#define ID_CHOICE_BILL    XRCID("choiceBill")

#endif /* _GUI_DEFS_H_ */
