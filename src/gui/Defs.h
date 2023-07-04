#ifndef _GUI_DEFS_H_
#define _GUI_DEFS_H_

#include <wx/xrc/xmlres.h>

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

// View
#define ID_BOOK         XRCID("book")
#define ID_BOOK_CONFIGS XRCID("bookConfigs")

// Data Panel
#define ID_DATE_DATA XRCID("dateData")

// Paste Bill Dialog
#define ID_CHOICE_ACCOUNT XRCID("choiceAccount")

// Bill Panel
#define ID_CHOICE_BILL_ACCOUNT XRCID("choiceBillAccount")
#define ID_CHOICE_BILL_TITLE   XRCID("choiceBillTitle")
#define ID_BTN_PASTE_BILL      XRCID("btnPasteBill")
#define ID_BTN_MERGE           XRCID("btnMerge")

#endif /* _GUI_DEFS_H_ */
