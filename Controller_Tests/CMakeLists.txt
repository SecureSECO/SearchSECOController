# Required by FetchContent_MakeAvailable
cmake_minimum_required(VERSION 3.13)
project(controller)

add_subdirectory(../Controller ./Controller)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

set(CMAKE_CXX_STANDARD 20)
set(TEST_NAME controller_tests)
add_executable(${TEST_NAME} Commands_tests.cpp Print_tests.cpp Utils_tests.cpp)
target_link_libraries(${TEST_NAME} gtest_main controller)

target_include_directories(${TEST_NAME} PUBLIC "${PROJECT_BINARY_DIR}" ../Controller)

enable_testing()
include(GoogleTest)
gtest_discover_tests(${TEST_NAME})