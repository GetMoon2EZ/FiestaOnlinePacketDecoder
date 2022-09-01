set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_library(IMGUI STATIC)

target_sources(
    IMGUI
    PRIVATE
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui.cpp

    PRIVATE
    ${IMGUI_DIR}/backends/imgui_impl_win32.cpp
    ${IMGUI_DIR}/backends/imgui_impl_dx12.cpp
)

target_include_directories(
    IMGUI
    PUBLIC ${IMGUI_DIR}
    PUBLIC ${IMGUI_DIR}/backends
)

target_compile_definitions(
    IMGUI
    PUBLIC
    BUILD_IMGUI_SHARED
    -DImTextureID=ImU64
)