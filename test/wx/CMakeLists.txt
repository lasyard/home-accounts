find_package(wxWidgets REQUIRED COMPONENTS core base xml xrc adv)

set(TEST_MAIN_SRC ../test.cpp)

include(${wxWidgets_USE_FILE})

add_test(TestWxString ${TEST_MAIN_SRC} TestWxString.cpp)
target_link_libraries(TestWxString PRIVATE ${wxWidgets_LIBRARIES})
