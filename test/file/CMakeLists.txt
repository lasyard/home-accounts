include_directories(${CMAKE_SOURCE_DIR}/src/file)

set(TEST_MAIN_SRC ../test.cpp)

set(LIB_FILE file)

add_test(TestCache ${TEST_MAIN_SRC} TestCache.cpp)
target_link_libraries(TestCache PRIVATE ${LIB_FILE})

add_test(TestCrypto ${TEST_MAIN_SRC} TestCrypto.cpp)
target_link_libraries(TestCrypto PRIVATE ${LIB_FILE})

add_test(TestFileStore ${TEST_MAIN_SRC} TestFileStore.cpp)
target_link_libraries(TestFileStore PRIVATE ${LIB_FILE})

add_test(TestSectionRecord ${TEST_MAIN_SRC} TestSectionRecord.cpp)
target_link_libraries(TestSectionRecord PRIVATE ${LIB_FILE})
