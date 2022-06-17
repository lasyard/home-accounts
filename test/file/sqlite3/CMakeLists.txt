add_test(TestSqlite3File ../../test.cpp TestSqlite3File.cpp)
target_link_libraries(TestSqlite3File PRIVATE file_sqlite3)
