include_directories(${CMAKE_SOURCE_DIR}/src/file)

add_test(TestCrypto ../test.cpp TestCrypto.cpp)
target_link_libraries(TestCrypto PRIVATE ${LIB_CRYPTO_NAME})
add_test(TestSectionFile ../test.cpp TestSectionFile.cpp)
target_link_libraries(TestSectionFile PRIVATE ${LIB_FILE_NAME} file_simple)

add_subdirectory(directory)
add_subdirectory(simple)
