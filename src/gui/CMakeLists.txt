find_package(wxWidgets REQUIRED COMPONENTS core base xml xrc adv)

include(${wxWidgets_USE_FILE})

set(APP_NAME HomeAccounts)
add_compile_definitions(APP_NAME="${APP_NAME}")
set(XRS_FILE resource.xrs)
add_compile_definitions(XRS_FILE="${XRS_FILE}")

set(XRC_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/ha.xrc)
add_custom_command(
    OUTPUT res/${XRS_FILE}
    COMMAND wxrc ${XRC_SRC_FILES} -o res/${XRS_FILE}
    DEPENDS ${XRC_SRC_FILES}
)
set(RESOURCE_FILES
    res/LICENSE
    res/images/logo.png
    res/${XRS_FILE}
)

if(APPLE)
    set(ICNS_FILE macos.icns)
    set(ICNS_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res/macos.iconset)
    add_custom_command(
        OUTPUT res/${ICNS_FILE}
        COMMAND iconutil -c icns -o res/${ICNS_FILE} ${ICNS_SRC_DIR}
        DEPENDS ${ICNS_SRC_DIR}
    )
    set(RESOURCE_FILES ${RESOURCE_FILES} res/${ICNS_FILE})
endif()

add_exe(${APP_NAME}
    ${RESOURCE_FILES}
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
add_subdirectory(bill)
add_subdirectory(configs)
add_subdirectory(data)
add_subdirectory(raw)
add_subdirectory(res)
add_subdirectory(utils)

target_link_libraries(${APP_NAME} PRIVATE ${wxWidgets_LIBRARIES})
target_link_libraries(${APP_NAME} PRIVATE ${LIB_FILE_NAME})
target_link_libraries(${APP_NAME} PRIVATE ${LIB_DATA_NAME})
target_link_libraries(${APP_NAME} PRIVATE file_sqlite3)
target_link_libraries(${APP_NAME} PRIVATE csv)
set_target_properties(${APP_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_FRAMEWORK_IDENTIFIER io.github.lasyard.${APP_NAME}
    RESOURCE "${RESOURCE_FILES}"
)
