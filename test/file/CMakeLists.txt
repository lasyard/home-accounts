include_directories(${CMAKE_SOURCE_DIR}/src/file)

add_test(TestCrypto ../test.cpp TestCrypto.cpp)
target_link_libraries(TestCrypto PRIVATE ${FILE_LIB_NAME})
