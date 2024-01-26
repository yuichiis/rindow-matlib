# cmakeのバージョン
cmake_minimum_required(VERSION 3.13)

# プロジェクト名指定
project(rindowmatlib LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


find_package(Threads REQUIRED)

file(GLOB_RECURSE srcfiles RELATIVE ${PROJECT_SOURCE_DIR} ./*.c)

add_library(rindowmatlib SHARED ${srcfiles})
target_compile_options(rindowmatlib PRIVATE -DRINDOW_COMPILING_DLL)
#find_package(OpenMP REQUIRED)
#if(OpenMP_FOUND)
#    target_link_libraries(rindowmatlib PRIVATE OpenMP::OpenMP_C)
#endif()
if (MSVC)
    target_compile_options(rindowmatlib PRIVATE /openmp:experimental)
elseif(UNIX)
    target_compile_options(rindowmatlib PRIVATE -O2 -fopenmp -msse2)
endif()

target_include_directories(rindowmatlib PUBLIC ../include)
if (NOT MSVC)
    target_compile_options(rindowmatlib PRIVATE "-I${PROJECT_SOURCE_DIR}/../include")
endif()

# c++11
#target_compile_features(greetings PRIVATE cxx_std_11)


