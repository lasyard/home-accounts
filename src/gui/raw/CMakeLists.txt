set(APP_NAME HaRawView)
add_compile_definitions(APP_NAME="${APP_NAME}")
set(XRS_FILE resource.xrs)
add_compile_definitions(XRS_FILE="${XRS_FILE}")

set(XRC_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/res/raw.xrc)
add_custom_command(
    OUTPUT res/${XRS_FILE}
    COMMAND wxrc ${XRC_SRC_FILES} -o res/${XRS_FILE}
    DEPENDS ${XRC_SRC_FILES}
)

add_exe(${APP_NAME}
    res/${XRS_FILE}
    RawApp.cpp
    RawMainFrame.cpp
    RawView.cpp
    ../ChangePassDialogBase.cpp
    ../ChangePassDialog.cpp
    ../HaDocumentBase.cpp
    ../HaViewBase.cpp
)
target_link_libraries(${APP_NAME} PRIVATE ${wxWidgets_LIBRARIES})
target_link_libraries(${APP_NAME} PRIVATE ${LIB_FILE_NAME})
target_link_libraries(${APP_NAME} PRIVATE file_sqlite3)

add_subdirectory(res)
