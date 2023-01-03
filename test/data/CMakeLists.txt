include_directories(${CMAKE_SOURCE_DIR}/src/data)

add_test(test_defs ../test.cpp test_defs.cpp)
add_test(test_list ../test.cpp test_list.cpp)
target_link_libraries(test_list PRIVATE ${LIB_DATA_NAME})
add_test(test_rbtree ../test.cpp test_rbtree.cpp)
target_link_libraries(test_rbtree PRIVATE ${LIB_DATA_NAME})
add_test(TestDataFile ../test.cpp TestDataFile.cpp)
target_link_libraries(TestDataFile PRIVATE ${LIB_DATA_NAME} ${LIB_CSV_NAME})
