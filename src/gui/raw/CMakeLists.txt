set(APP_NAME HaRawView)
add_compile_definitions(APP_NAME="${APP_NAME}")
set(XRS_FILE resource.xrs)
add_compile_definitions(XRS_FILE="${XRS_FILE}")

add_exe(${APP_NAME}
    ChangePassDialogBase.cpp
    ChangePassDialog.cpp
    RawApp.cpp
    RawDocument.cpp
    RawMainFrame.cpp
    RawView.cpp
)
target_link_libraries(${APP_NAME} PRIVATE ${wxWidgets_LIBRARIES})
target_link_libraries(${APP_NAME} PRIVATE ${LIB_FILE_NAME})
target_link_libraries(${APP_NAME} PRIVATE file_sqlite3)

add_subdirectory(res)
