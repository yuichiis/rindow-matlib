
file(GLOB_RECURSE testfiles.thread RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ./*.cpp)

add_executable(test_thread ${testfiles.thread})

target_link_libraries(test_thread GTest::gtest_main)
if(MSVC)
  target_link_libraries(test_thread rindowmatlib.thread)
else()
  target_link_libraries(test_thread rindowmatlib.thread)
endif()
target_include_directories(test_thread PUBLIC ${gmock_SOURCE_DIR}/include)

add_test(
  NAME thread_test
  COMMAND test_thread
  CONFIGURATIONS Release
)

