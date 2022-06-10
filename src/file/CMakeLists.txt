add_lib(${FILE_LIB_NAME}
    Crypto.cpp
)
target_link_libraries(${FILE_LIB_NAME} PUBLIC libcryptopp.a)
