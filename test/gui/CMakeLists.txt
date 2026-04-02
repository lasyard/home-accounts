find_package(wxWidgets REQUIRED COMPONENTS core base xml xrc adv)
include_directories(${CMAKE_SOURCE_DIR}/src/gui)

set(TEST_MAIN_SRC ../test.cpp)

include(${wxWidgets_USE_FILE})

add_doctest(TestHaDoc ${TEST_MAIN_SRC} TestHaDoc.cpp LIBS ${wxWidgets_LIBRARIES} csv)
target_sources(TestHaDoc PRIVATE
    ${CMAKE_SOURCE_DIR}/src/gui/HaCsv.cpp
    ${CMAKE_SOURCE_DIR}/src/gui/Utils.cpp
)
copy_file(sample.csv)
copy_file(sample1.csv)
target_sources(TestHaDoc PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}/sample.csv
    ${CMAKE_CURRENT_BINARY_DIR}/sample1.csv
)

add_doctest(TestBiMap ${TEST_MAIN_SRC} TestBiMap.cpp)
add_doctest(TestUtils ${TEST_MAIN_SRC} TestUtils.cpp)
