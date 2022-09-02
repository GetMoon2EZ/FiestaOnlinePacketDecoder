cmake_minimum_required(VERSION 3.9)
set(MAJOR_VERSION 1)

set(PROJECT_NAME "fiesta_online_packet_decoder")

project(${PROJECT_NAME} VERSION 1.0.0 DESCRIPTION "DPS meter for Fiesta Online")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#######################
# Include directories #
#######################

include_directories(
    $(CMAKE_SOURCE_DIR)
    ../include/
    ../vendor/imgui/
    ../vendor/imgui/backends/
)

#################
# C and H files # 
#################

set(SOURCE_FILES
    "main.cpp"

    "utils/consts.cpp"
    "utils/utils.cpp"
    "utils/data.cpp"

    "packets/damage.cpp"
    "packets/entity_stats.cpp"
    "packets/packet.cpp"

    "threads/dps_meter.cpp"
    "threads/packet_sniffer.cpp"

    "gui/gui.cpp"
)

#####################
# Compilation flags #
#####################
if(MSVC)
    # Don't always use Wall, since VC's /Wall is ridiculously verbose.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
    # Disable VC secure checks, since these are not really issues.
    add_definitions("-D_CRT_SECURE_NO_WARNINGS=1")
    add_definitions("-D_SCL_SECURE_NO_WARNINGS=1")
    add_definitions("-DNOGDI=1")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

if (TEST)
    SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
else()
    add_executable(${PROJECT_NAME} ${SOURCE_FILES})
endif()

add_compile_definitions(TINS_STATIC)

##################
# Link libraries #
##################
target_link_directories(
    ${PROJECT_NAME}
    PUBLIC
    ../lib
)

target_link_libraries(
    ${PROJECT_NAME}
    # Libtins linkage
    Ws2_32 Iphlpapi wpcap tins
    # IMGUI linkage 
    D3d12 DXGI IMGUI
)