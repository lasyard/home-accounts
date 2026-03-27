#ifndef _HA_GUI_HA_DEFS_H_
#define _HA_GUI_HA_DEFS_H_

#include <wx/xrc/xmlres.h>

// menus
#define ID_CHANGE_PASS XRCID("changePass")
#define ID_INSERT      XRCID("insert")
#define ID_IMPORT      XRCID("import")
#define ID_ABOUT       XRCID("about")
#define ID_LICENSE     XRCID("license")
#define ID_WX_INFO     XRCID("wxInfo")

// config
constexpr const int MAX_BACKUPS = 5;
constexpr const char CFG_FILE_HISTORY[] = "FileHistory";
constexpr const char LICENSE_FILE_NAME[] = "license.txt";

// arts
constexpr const char LASYARD_LOGO[] = "lasyard_logo.png";
constexpr const char APP_ICON[] = "app_icon.png";

// sections
constexpr const char ACCOUNTS_SECTION_NAME[] = "accounts";
constexpr const char IMPORT_COL_MAP_SECTION_NAME[] = "conf/import_col_map";
constexpr const char DATA_SECTION_NAME_FORMAT[] = "data/%d";
constexpr const char IMPORT_SECTION_NAME[] = "import";

#endif /* _HA_GUI_HA_DEFS_H_ */
