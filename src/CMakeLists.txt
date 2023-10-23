####################
# Add source files #
####################

target_sources(
    ${PROJECT_NAME}
    PUBLIC
    "main.cpp"

    "utils/utils.cpp"
    "utils/data.cpp"
    "utils/translation.cpp"
    "utils/data_stream.cpp"

    "packets/packet.cpp"
    "packets/network_stash.cpp"

    "threads/packet_sniffer.cpp"
    "threads/timer.cpp"
    "threads/ping.cpp"

    "gui/gui.cpp"
    "gui/gui_helper.cpp"
)
