include_directories(${CMAKE_SOURCE_DIR}/src/csv)

set(TEST_MAIN_SRC ../test.cpp)

set(LIB_CSV csv)

add_test(test_csv_parser ${TEST_MAIN_SRC} test_csv_parser.cpp LIBS ${LIB_CSV})
add_test(test_date_time ${TEST_MAIN_SRC} test_date_time.cpp LIBS ${LIB_CSV})
add_test(test_defs ${TEST_MAIN_SRC} test_defs.cpp)
add_test(test_int ${TEST_MAIN_SRC} test_int.cpp LIBS ${LIB_CSV})
add_test(test_list ${TEST_MAIN_SRC} test_list.cpp LIBS ${LIB_CSV})
add_test(test_money ${TEST_MAIN_SRC} test_money.cpp LIBS ${LIB_CSV})
add_test(test_str ${TEST_MAIN_SRC} test_str.cpp LIBS ${LIB_CSV})
add_test(test_utils ${TEST_MAIN_SRC} test_utils.cpp)
