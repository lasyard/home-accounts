find_package(wxWidgets REQUIRED COMPONENTS core base xml xrc adv)
find_package(Gettext REQUIRED)

include(${wxWidgets_USE_FILE})

set(APP_NAME HomeAccounts)
add_compile_definitions(APP_NAME="${APP_NAME}")
set(APP_VERSION ${CMAKE_PROJECT_VERSION})
add_compile_definitions(APP_VERSION="${APP_VERSION}")
set(AUTHOR "Yueguang Jiao")
add_compile_definitions(AUTHOR="${AUTHOR}")
set(EMAIL "<lasyard@yeah.net>")
add_compile_definitions(EMAIL="${EMAIL}")
string(TIMESTAMP CURRENT_YEAR "%Y")
add_compile_definitions(CURRENT_YEAR="${CURRENT_YEAR}")
set(XRS_FILE resource.xrs)
add_compile_definitions(XRS_FILE="${XRS_FILE}")

add_exe(${APP_NAME}
    CachedTable.cpp
    ChangePassDialog.cpp
    HaApp.cpp
    HaArtProvider.cpp
    HaDocument.cpp
    HaGrid.cpp
    HaGridCellAttrProvider.cpp
    HaMainFrame.cpp
    HaPanel.cpp
    HaView.cpp
)

target_link_libraries(${APP_NAME}
    PRIVATE ${wxWidgets_LIBRARIES}
    PRIVATE ${LIB_CSV_NAME}
    PRIVATE ${LIB_DATA_NAME}
    PRIVATE ${LIB_FILE_NAME}
    PRIVATE file_sqlite3
)

# XRS
set(XRC_SRC_FILES res/ha.xrc)

# This must be in this file to generate dependencies.
add_custom_command(
    OUTPUT res/${XRS_FILE}
    COMMAND wxrc ${CMAKE_CURRENT_SOURCE_DIR}/${XRC_SRC_FILES} -o res/${XRS_FILE}
    DEPENDS ${XRC_SRC_FILES}
)

# Add to source to make it appear in bundle resource.
target_sources(${APP_NAME} PRIVATE res/${XRS_FILE})

# macOS icon
if(APPLE)
    set(ICNS_FILE macos.icns)
    set(ICNS_SRC_DIR res/macos.iconset)
    file(GLOB ICNS_SRC_FILES RELATIVE ICNS_SRC_DIR CONFIGURE_DEPENDS *.png)
    add_custom_command(
        OUTPUT res/${ICNS_FILE}
        COMMAND iconutil -c icns -o res/${ICNS_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/${ICNS_SRC_DIR}
        DEPENDS ${ICNS_SRC_FILES}
    )
    target_sources(${APP_NAME} PRIVATE res/${ICNS_FILE})
endif()

set_target_properties(${APP_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_FRAMEWORK_IDENTIFIER io.github.lasyard.${APP_NAME}
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/res/MacOSXBundleInfo.plist.in
)

if(APPLE)
    set_source_files_properties(
        res/LICENSE
        res/images/logo.png
        res/${XRS_FILE}
        res/${ICNS_FILE}
        PROPERTIES MACOSX_PACKAGE_LOCATION Resources
    )
endif()

# Languages
set(PO_DIR res/i18n)
GETTEXT_PROCESS_PO_FILES(en ALL PO_FILES ${PO_DIR}/en.po)
target_sources(${APP_NAME} PRIVATE en.gmo)
GETTEXT_PROCESS_PO_FILES(zh ALL PO_FILES ${PO_DIR}/zh.po)
target_sources(${APP_NAME} PRIVATE zh.gmo)

if(APPLE)
    set_source_files_properties(en.gmo PROPERTIES MACOSX_PACKAGE_LOCATION Resources/en.lproj)
    set_source_files_properties(zh.gmo PROPERTIES MACOSX_PACKAGE_LOCATION Resources/zh.lproj)
endif()

add_subdirectory(bill)
add_subdirectory(configs)
add_subdirectory(data)
add_subdirectory(raw)
add_subdirectory(res)
add_subdirectory(utils)
