set(APP_NAME HomeAccounts)
add_compile_definitions(APP_NAME="${APP_NAME}")
set(XRS_FILE resource.xrs)
add_compile_definitions(XRS_FILE="${XRS_FILE}")

set(XRC_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/ha.xrc)
add_custom_command(
    OUTPUT res/${XRS_FILE}
    COMMAND wxrc ${XRC_SRC_FILES} -o ${XRS_FILE}
    DEPENDS ${XRC_SRC_FILES}
)
set(RESOURCE_FILES
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
    HaApp.cpp
    HaMainFrame.cpp
    HaDocument.cpp
    HaView.cpp
    DataGrid.cpp
    DataTable.cpp
    ../ChangePassDialogBase.cpp
    ../ChangePassDialog.cpp
    ../HaDocumentBase.cpp
    ../HaViewBase.cpp
    ../HaArtProvider.cpp
)
target_link_libraries(${APP_NAME} PRIVATE ${wxWidgets_LIBRARIES})
target_link_libraries(${APP_NAME} PRIVATE ${LIB_FILE_NAME})
target_link_libraries(${APP_NAME} PRIVATE file_sqlite3)
target_link_libraries(${APP_NAME} PRIVATE csv)
set_target_properties(${APP_NAME} PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_FRAMEWORK_IDENTIFIER io.github.lasyard.${APP_NAME}
    RESOURCE "${RESOURCE_FILES}"
)

add_subdirectory(res)
