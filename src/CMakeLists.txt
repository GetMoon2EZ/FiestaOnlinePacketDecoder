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

    "packets/packet.cpp"

    "threads/packet_sniffer.cpp"
    "threads/timer.cpp"
    "threads/ping.cpp"

    "gui/gui.cpp"
    "gui/gui_helper.cpp"
)
