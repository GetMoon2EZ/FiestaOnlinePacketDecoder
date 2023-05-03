cmake_minimum_required(VERSION 3.26)
set(MAJOR_VERSION 1)

set(PROJECT_NAME "fiesta_online_packet_decoder")

project(${PROJECT_NAME} VERSION 1.0.0 DESCRIPTION "DPS meter for Fiesta Online")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Executable output path
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

if (TEST)
    enable_testing()
endif()

add_subdirectory(vendor)
add_subdirectory(src)

if (TEST)
    add_subdirectory(test)
endif()