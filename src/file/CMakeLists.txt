include_directories(${CMAKE_SOURCE_DIR}/external/sqlite-amalgamation-3510000)

add_lib(file
    Cache.cpp
    Crypto.cpp
    CryptoStore.cpp
    FileStore.cpp
    SectionRecord.cpp
)

target_link_libraries(file PUBLIC libcryptopp.a sqlite3)
