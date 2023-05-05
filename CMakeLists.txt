cmake_minimum_required(VERSION 3.26)
set(MAJOR_VERSION 1)

set(PROJECT_NAME "fiesta_online_packet_decoder")

project(${PROJECT_NAME} VERSION 1.0.0 DESCRIPTION "DPS meter for Fiesta Online")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Executable output path
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

if (DEBUG)
    add_executable(${PROJECT_NAME})
    target_compile_definitions(${PROJECT_NAME} PUBLIC FOPD_DEBUG)
else()
    add_executable(${PROJECT_NAME} WIN32)
endif()

#####################
# Compilation flags #
#####################

if(MSVC)
    # Don't always use Wall, since VC's /Wall is ridiculously verbose.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
    # Disable VC secure checks, since these are not really issues.
    add_definitions("-D_CRT_SECURE_NO_WARNINGS=1")
    add_definitions("-D_SCL_SECURE_NO_WARNINGS=1")
    # add_definitions("-DNOGDI=1")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

#######################
# Include directories #
#######################

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

##################
# Link libraries #
##################

target_link_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

########
# MISC #
########

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCH "x64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    # 32 bits
    set(ARCH "x86")
endif()
message("Compiling for ${ARCH}")

add_subdirectory(vendor)
add_subdirectory(src)
