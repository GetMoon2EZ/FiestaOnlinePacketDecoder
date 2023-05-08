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
    "gui/gui_helper.cpp"
)
