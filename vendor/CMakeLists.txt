include(ExternalProject)

#########
# ImGUI #
#########

# Builds itself
add_subdirectory(imgui)

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

# target_include_directories(
#     ${PROJECT_NAME}
#     PUBLIC
#     libtins/include
# )
