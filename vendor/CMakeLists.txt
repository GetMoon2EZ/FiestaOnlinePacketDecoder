include(ExternalProject)

#########
# ImGUI #
#########

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

target_sources(
    ${PROJECT_NAME}
    PUBLIC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp

    ${IMGUI_DIR}/backends/imgui_impl_dx12.cpp
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
)

target_compile_definitions(${PROJECT_NAME} PUBLIC STATIC_IMGUI)
target_link_libraries(${PROJECT_NAME} D3d12 DXGI)

###########
# WinPcap #
###########

# Pre-compiled
set(WINPCAP "WpdPack")

file(
    COPY
    ${CMAKE_CURRENT_SOURCE_DIR}/${WINPCAP}/Lib/x64/wpcap.lib
    DESTINATION ${PROJECT_SOURCE_DIR}/lib
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/WpdPack/Include
)

###########
# LibTins #
###########

# Build libtins as a static library
# See https://libtins.github.io/download/#compiling-windows for details
set(LIBTINS "libtins")

ExternalProject_Add(
    ${LIBTINS}
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${LIBTINS}
    BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${LIBTINS}/build
    CMAKE_ARGS -DPCAP_ROOT_DIR=${CMAKE_CURRENT_SOURCE_DIR}/${WINPCAP}/ -DLIBTINS_ENABLE_WPA2=0 -DLIBTINS_BUILD_SHARED=0
    INSTALL_COMMAND powershell.exe cp ${CMAKE_CURRENT_SOURCE_DIR}/${LIBTINS}/build/lib/Debug/tins.lib ${PROJECT_SOURCE_DIR}/lib
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libtins/include
)

target_compile_definitions(${PROJECT_NAME} PUBLIC TINS_STATIC)
target_link_libraries(${PROJECT_NAME} Ws2_32 Iphlpapi wpcap tins)
