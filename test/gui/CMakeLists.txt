find_package(wxWidgets REQUIRED COMPONENTS core base xml xrc adv)
include_directories(${CMAKE_SOURCE_DIR}/src/gui)

set(TEST_MAIN_SRC ../test.cpp)

include(${wxWidgets_USE_FILE})

add_doctest(TestCsvDoc ${TEST_MAIN_SRC} TestCsvDoc.cpp LIBS ${wxWidgets_LIBRARIES} csv)
target_sources(TestCsvDoc PRIVATE ${CMAKE_SOURCE_DIR}/src/gui/CsvDoc.cpp)
copy_file(sample.csv)
target_sources(TestCsvDoc PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/sample.csv)
