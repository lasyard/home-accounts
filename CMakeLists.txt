cmake_minimum_required(VERSION 3.21.0)
set(CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX12.sdk)
project(HomeAccounts VERSION 1.0.0 LANGUAGES C CXX)

include(cmake-common/functions.cmake)

using_doctest(doctest)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEV_DIR $ENV{HOME}/workspace/devel)
    include_directories(${DEV_DIR}/include)
    link_directories(${DEV_DIR}/lib)
    set(ENV{PATH} ${DEV_DIR}/bin:$ENV{PATH})
else()
    include_directories($ENV{HOME}/include)
    link_directories($ENV{HOME}/lib)
endif()

include_directories(external)
include_directories(src)

add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(test)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
