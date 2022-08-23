find_package(wxWidgets REQUIRED COMPONENTS core base xml xrc)

include(${wxWidgets_USE_FILE})

add_subdirectory(ha)
add_subdirectory(raw)
