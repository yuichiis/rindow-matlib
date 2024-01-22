# cmakeのバージョン
cmake_minimum_required(VERSION 3.13)

# プロジェクト名指定
project(matlib CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(Threads REQUIRED)

file(GLOB_RECURSE srcfiles RELATIVE ${PROJECT_SOURCE_DIR} ./*.c)

add_compile_options(-DRINDOW_COMPILING_DLL)
if (MSVC)
    add_compile_options(/openmp:experimental)
endif()
add_library(matlib SHARED ${srcfiles})

# c++11
#target_compile_features(greetings PRIVATE cxx_std_11)

##target_include_directories(greetings INTERFACE ../include)
target_include_directories(matlib PUBLIC ../include)
