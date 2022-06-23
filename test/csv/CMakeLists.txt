include_directories(${CMAKE_SOURCE_DIR}/src/csv)

add_test(test_csv ../test.cpp test_csv.cpp)
target_link_libraries(test_csv PRIVATE ${LIB_CSV_NAME})
add_test(test_int ../test.cpp test_int.cpp)
target_link_libraries(test_int PRIVATE ${LIB_CSV_NAME})
add_test(test_str ../test.cpp test_str.cpp)
target_link_libraries(test_str PRIVATE ${LIB_CSV_NAME})
