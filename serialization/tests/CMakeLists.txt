cmake_minimum_required(VERSION 3.12)


# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/6f01e3dc125e82f2d3d87839a8be52f610c732ea.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable(
  hello_test
  tests.cpp
)
target_link_libraries(
  hello_test
  GTest::gtest_main
  gmock_main
  example_google_tests
)

include(GoogleTest)
gtest_discover_tests(hello_test)