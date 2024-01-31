#
# Type to Build two commannds on command line. 
#
# to configure
#  > cmake -S . -B build
#
# to build
#  > cmake --build build --config Debug
#   or
#  > cmake --build build --config Release

cmake_minimum_required(VERSION 3.14)
#include (CMakePrintSystemInformation)
enable_testing()

set(CPACK_PACKAGE_NAME "rindow-matlib")
set(CPACK_PACKAGE_VERSION "0.1.0")
set(CPACK_PACKAGE_DESCRIPTION "Rindow Math Matrix library")
set(CPACK_PACKAGE_CONTACT "Rindow Developers <rindow@users.noreply.github.com>")
set(CPACK_PACKAGE_LICENSE "BSD3-Clause")
include(CPack)
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.7)")

#set(CMAKE_CONFIGURATION_TYPES Debug Release)
##set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
##set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

project(rindow_matlib VERSION 0.1.0 LANGUAGES CXX C)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(./src)
add_subdirectory(./include)
add_subdirectory(./tests)
if(UNIX)
    add_subdirectory(./pkgconfig)
endif()
