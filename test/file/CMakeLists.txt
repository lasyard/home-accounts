include_directories(${CMAKE_SOURCE_DIR}/src/file)

set(TEST_MAIN_SRC ../test.cpp)

set(LIB_FILE file)

add_test(TestCache ${TEST_MAIN_SRC} TestCache.cpp LIBS ${LIB_FILE})

add_test(TestCrypto ${TEST_MAIN_SRC} TestCrypto.cpp LIBS ${LIB_FILE})

add_test(TestSectionRecord ${TEST_MAIN_SRC} TestSectionRecord.cpp LIBS ${LIB_FILE})

add_test(TestStores ${TEST_MAIN_SRC} TestStores.cpp LIBS ${LIB_FILE})
