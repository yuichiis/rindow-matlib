cmake_minimum_required(VERSION 3.14)

project(test_greetings CXX)

## If you want to build the Release version, type the following command
## cmake -B build -DCMAKE_BUILD_TYPE=Release
#set(CMAKE_CONFIGURATION_TYPES Debug Release)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
message(STATUS "gtest_SOURCE_DIR = ${gtest_SOURCE_DIR}")
message(STATUS "gmock_SOURCE_DIR = ${gmock_SOURCE_DIR}")

enable_testing()

file(GLOB_RECURSE testfiles RELATIVE ${PROJECT_SOURCE_DIR} ./*.cpp)

#add_executable(test_main test_main.cpp)
add_executable(test_main ${testfiles})
target_link_libraries(test_main GTest::gtest_main)
target_link_libraries(test_main matlib)
target_include_directories(test_main PUBLIC ${gmock_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(test_main)

find_package(Threads REQUIRED)

add_executable(omptest omptest.c)
target_link_libraries(omptest matlib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
