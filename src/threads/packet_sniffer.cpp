#include "fopd/packet_sniffer.h"

#include <stdio.h>
#include <tins/tins.h>

#include "fopd/fopd_consts.h"
#include "fopd/fopd_packet.h"
#include "fopd/fopd_data.h"

using namespace Tins;

static Sniffer configure_sniffer(size_t server);

bool process_packet(PDU& pkt)
{
    try {
        // Extract raw TCP data from the packet
        const RawPDU &raw = pkt.rfind_pdu<RawPDU>();
        auto payload = raw.payload();
        uint8_t *data = payload.data();
        uint32_t data_len = raw.payload_size();
        uint32_t current_pos = 0;
        uint32_t payload_len = 0;
        struct fopacket packet;

        while (current_pos < data_len && get_payload_len(&data[current_pos], data_len - current_pos, &payload_len) != 1) {
            if (payload_len == 0) {
                fprintf(stderr, "[ERROR] Empty payload\n");
                break;
            }

            if (parse_packet(&data[current_pos], payload_len, &packet) != 0) {
                fprintf(stderr, "[ERROR] Fail to parse packet\n");
                break;
            }

            switch (packet.type) {
                case FOPACKET_DMG_AA:
                case FOPACKET_DMG_SPELL:
                    handle_damage(&packet);
                    break;
                case FOPACKET_ENTITY_INFO:
                    handle_entity_info(&packet);
                    break;
                case FOPACKET_FRIEND_FIND:
                    handle_friend_find(&packet);
                    break;
                default:
                    /* Nothing to do */
                    print_packet(&packet);
                    break;
            }
            current_pos += payload_len;
        }
    }
    catch (pdu_not_found error) {
        return false;
    }
    return true;
}

void sniffer_thread(void)
{
    FOPDData *data = FOPDData::getInstance();
    size_t current_server = data->getServerIndex();
    size_t old_server = current_server;

    std::cout << "Server IP (default): " << fo_servers[current_server].address << std::endl;

    Sniffer sniffer = configure_sniffer(current_server);

    while(1) {
        current_server = data->getServerIndex();
        if (current_server != old_server) {
            std::cout << "[DEBUG] Changing capture filter" << std::endl;
            sniffer = configure_sniffer(current_server);
            old_server = current_server;
        }
        sniffer.sniff_loop(&process_packet, 1);
    }
}

static Sniffer configure_sniffer(size_t server)
{
    char filter[256] = { 0 };
    SnifferConfiguration config;
    NetworkInterface iface = NetworkInterface::default_interface();

    // Configure sniffer
    config.set_promisc_mode(true);
    snprintf(filter, sizeof(filter), "ip src %s", fo_servers[server].address);
    config.set_filter(filter);

    // Only inbound packets
    config.set_direction(PCAP_D_IN);
    config.set_snap_len(65535);
    config.set_timeout(1000);

    Sniffer sniffer = Sniffer(iface.name(), config);
    /* Use pcap_dispatch so we can timeout if sniffing on the wrong address */
    sniffer.set_pcap_sniffing_method(pcap_dispatch);

    return sniffer;
}
