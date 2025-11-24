find_package(wxWidgets REQUIRED COMPONENTS core base xml xrc adv)
find_package(Gettext REQUIRED)

include(${wxWidgets_USE_FILE})

set(APP_NAME HomeAccounts)
add_compile_definitions("APP_NAME=\"${APP_NAME}\"")
set(APP_VERSION ${CMAKE_PROJECT_VERSION})
add_compile_definitions("APP_VERSION=\"${APP_VERSION}\"")
set(AUTHOR "Yueguang Jiao")
add_compile_definitions("AUTHOR=\"${AUTHOR}\"")
set(EMAIL "<lasyard@yeah.net>")
add_compile_definitions("EMAIL=\"${EMAIL}\"")
set(XRS_FILE resource.xrs)
add_compile_definitions("XRS_FILE=\"${XRS_FILE}\"")
string(TIMESTAMP CURRENT_YEAR "%Y")

add_exe(${APP_NAME}
    ChangePassDialog.cpp
    HaApp.cpp
    HaArtProvider.cpp
    HaDocument.cpp
    HaMainFrame.cpp
    HaView.cpp
    Utils.cpp
)

# order is vital
target_link_libraries(${APP_NAME}
    PRIVATE ${wxWidgets_LIBRARIES} file
)

# XRS
set(XRC_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/res/widgets.xrc
    ${CMAKE_CURRENT_SOURCE_DIR}/res/menu_bar.xrc
)

# this must be in this file to generate dependencies
add_custom_command(
    OUTPUT res/${XRS_FILE}
    COMMAND wxrc ${XRC_SRC_FILES} -o res/${XRS_FILE}
    DEPENDS ${XRC_SRC_FILES}
)

# add to source to make it appear in bundle resource
target_sources(${APP_NAME} PRIVATE res/${XRS_FILE})

# this must be in this file to generate dependencies
copy_file(res/images/app_icon.png res/macos.iconset/icon_512x512.png)
# add to source to make it appear in bundle resource
target_sources(${APP_NAME} PRIVATE res/images/app_icon.png)

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
        res/images/lasyard_logo.png
        res/images/app_icon.png
        res/${XRS_FILE}
        res/${ICNS_FILE}
        PROPERTIES MACOSX_PACKAGE_LOCATION Resources
    )
endif()

# languages
set(PO_DIR res/i18n)

foreach(domain IN ITEMS ${APP_NAME} "xrc")
    foreach(locale IN ITEMS en zh)
        file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PO_DIR}/${locale})
        set(PO_FILE ${PO_DIR}/${domain}_${locale}.po)
        set(MO_FILE ${PO_DIR}/${locale}/${domain}.mo)

        # Target file name of `GETTEXT_PROCESS_PO_FILES` is not ok, so use custom commands.
        add_custom_command(
            OUTPUT ${MO_FILE}
            COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${MO_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/${PO_FILE}
            DEPENDS ${PO_FILE}
        )
        target_sources(${APP_NAME} PRIVATE ${MO_FILE})

        if(APPLE)
            set_source_files_properties(${MO_FILE} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/${locale}.lproj)
        endif()
    endforeach()
endforeach()

add_subdirectory(raw)
add_subdirectory(res)
