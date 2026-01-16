include_directories(${CMAKE_SOURCE_DIR}/external/sqlite-amalgamation-3510000)

add_lib(file
    Cache.cpp
    Crypto.cpp
    CryptoStore.cpp
    DirectoryStore.cpp
    FileStore.cpp
    SectionRecord.cpp
    Sqlite3Store.cpp
    Store.cpp
    LIBS libcryptopp.a sqlite3
)
