add_lib(${LIB_CRYPTO_NAME} Crypto.cpp)
target_link_libraries(${LIB_CRYPTO_NAME} PUBLIC libcryptopp.a)
add_lib(${LIB_FILE_NAME} SectionFile.cpp)

add_subdirectory(directory)
add_subdirectory(simple)
