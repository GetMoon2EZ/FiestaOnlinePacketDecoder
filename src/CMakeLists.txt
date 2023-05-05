####################
# Add source files #
####################

target_sources(
    ${PROJECT_NAME}
    PUBLIC
    "main.cpp"

    "utils/consts.cpp"
    "utils/utils.cpp"
    "utils/data.cpp"

    "packets/damage.cpp"
    "packets/entity_stats.cpp"
    "packets/packet.cpp"

    "threads/dps_meter.cpp"
    "threads/packet_sniffer.cpp"
    "threads/ping.cpp"

    "gui/gui.cpp"
)

target_compile_definitions(${PROJECT_NAME} PUBLIC TINS_STATIC)

target_link_libraries(
    ${PROJECT_NAME}
    # Libtins linkage
    Ws2_32 Iphlpapi wpcap tins
    # IMGUI linkage
    D3d12 DXGI
    IMGUI
)