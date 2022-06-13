set(LIB_NAME file_simple)

add_lib(${LIB_NAME}
    CatalogRecord.cpp
    CryptoFile.cpp
)
target_link_libraries(${LIB_NAME} PUBLIC ${LIB_CRYPTO_NAME})
