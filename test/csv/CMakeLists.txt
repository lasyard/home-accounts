include_directories(${CMAKE_SOURCE_DIR}/src/csv)

set(TEST_MAIN_SRC ../test.cpp)

set(LIB_CSV csv)

add_test(test_csv_parser ${TEST_MAIN_SRC} test_csv_parser.cpp)
target_link_libraries(test_csv_parser PRIVATE ${LIB_CSV})

add_test(test_date_time ${TEST_MAIN_SRC} test_date_time.cpp)
target_link_libraries(test_date_time PRIVATE ${LIB_CSV})

add_test(test_defs ${TEST_MAIN_SRC} test_defs.cpp)

add_test(test_int ${TEST_MAIN_SRC} test_int.cpp)
target_link_libraries(test_int PRIVATE ${LIB_CSV})

add_test(test_list ${TEST_MAIN_SRC} test_list.cpp)
target_link_libraries(test_list PRIVATE ${LIB_CSV})

add_test(test_money ${TEST_MAIN_SRC} test_money.cpp)
target_link_libraries(test_money PRIVATE ${LIB_CSV})

add_test(test_segmental_csv_parser ${TEST_MAIN_SRC} test_segmental_csv_parser.cpp)
target_link_libraries(test_segmental_csv_parser PRIVATE ${LIB_CSV})

add_test(test_str ${TEST_MAIN_SRC} test_str.cpp)
target_link_libraries(test_str PRIVATE csv)
