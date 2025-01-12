include_directories(${CMAKE_SOURCE_DIR}/external/sqlite-amalgamation-3460100)

add_lib(file
    Cache.cpp
    Crypto.cpp
    DirectoryStore.cpp
    FileStore.cpp
    SectionRecord.cpp
    Sqlite3Store.cpp
)

target_link_libraries(file PUBLIC libcryptopp.a sqlite3)
