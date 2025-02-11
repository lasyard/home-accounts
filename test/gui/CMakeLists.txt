include_directories(${CMAKE_SOURCE_DIR}/src/gui)

set(TEST_MAIN_SRC ../test.cpp)

add_test(TestAlgos ${TEST_MAIN_SRC} TestAlgos.cpp)
