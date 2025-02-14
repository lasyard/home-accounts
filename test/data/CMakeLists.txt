include_directories(${CMAKE_SOURCE_DIR}/src/data)

set(TEST_MAIN_SRC ../test.cpp)

set(LIB_DATA data)
set(LIB_CSV csv)

add_test(test_data ${TEST_MAIN_SRC} test_data.cpp)
target_link_libraries(test_data PRIVATE ${LIB_DATA} ${LIB_CSV})
