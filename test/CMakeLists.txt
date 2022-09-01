cmake_minimum_required(VERSION 3.14)
project(fopd_test)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include_directories(
    ../include/
)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(
  main_test
  main_test.cpp
)

target_link_libraries(
  main_test
  GTest::gtest_main
  fiesta_online_packet_decoder
)

include(GoogleTest)
# add_test(main_gtest main_test)
gtest_discover_tests(main_test)