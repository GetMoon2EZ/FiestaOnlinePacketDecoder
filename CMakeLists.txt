cmake_minimum_required(VERSION 3.9)
set(MAJOR_VERSION 1)

set(PROJECT_NAME "fiesta_online_packet_decoder")

project(${PROJECT_NAME} VERSION 1.0.0 DESCRIPTION "DPS meter for Fiesta Online")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (TEST)
    enable_testing()
endif()

add_subdirectory(vendor/imgui)
add_subdirectory(src)

if (TEST)
    add_subdirectory(test)
endif()