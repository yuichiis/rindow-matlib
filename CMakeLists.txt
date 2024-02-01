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


#set(CMAKE_CONFIGURATION_TYPES Debug Release)
##set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
##set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)

project(rindow-matlib VERSION 0.1.0 LANGUAGES CXX C)
#set(PROJECT_VERSION_MAJOR 0)
#set(PROJECT_VERSION_MINOR 1)
#set(PROJECT_VERSION_PATCH 0)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(./src)
add_subdirectory(./include)
add_subdirectory(./tests)

if(UNIX)
    add_subdirectory(./debian)
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    include(Packing)
endif()

message(STATUS "Components to pack: ${CPACK_COMPONENTS_ALL}")
message(STATUS "Package description file: ${CPACK_PACKAGE_DESCRIPTION_FILE}")
message(STATUS "CMAKE_SYSTEM: ${CMAKE_SYSTEM}")
message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_SYSTEM_VERSION: ${CMAKE_SYSTEM_VERSION}")
message(STATUS "CMAKE_HOST_SYSTEM: ${CMAKE_HOST_SYSTEM}")
message(STATUS "CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")
message(STATUS "CMAKE_HOST_SYSTEM_VERSION: ${CMAKE_HOST_SYSTEM_VERSION}")
message(STATUS "CPACK_PACKAGE_VERSION: ${CPACK_PACKAGE_VERSION}")

