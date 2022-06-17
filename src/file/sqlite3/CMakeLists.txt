set(LIB_NAME file_sqlite3)

add_lib(${LIB_NAME}
    Sqlite3File.cpp
)
target_link_libraries(${LIB_NAME} PUBLIC ${LIB_CRYPTO_NAME})
target_link_libraries(${LIB_NAME} PUBLIC sqlite3)
