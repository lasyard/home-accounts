add_test(TestCatalogRecord ../../test.cpp TestCatalogRecord.cpp)
target_link_libraries(TestCatalogRecord PRIVATE file_simple)
add_test(TestCryptoFile ../../test.cpp TestCryptoFile.cpp)
target_link_libraries(TestCryptoFile PRIVATE file_simple)
