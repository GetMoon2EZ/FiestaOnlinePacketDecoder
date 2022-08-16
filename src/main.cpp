#include <iostream>
#include <iomanip>

#include <utility>
#include <vector>

#include <tins/tins.h>

#include "fopd/fopd_types.h"
#include "fopd/fopd_utils.h"
#include "fopd/fopd_packet.h"
#include "fopd/fopd_packet_entity_stats.h"
#include "fopd/fopd_packet_damage.h"


using namespace Tins;

bool sniffer_callback(PDU& pkt)
{
    try {
        // Extract raw TCP data from the packet
        const RawPDU &raw = pkt.rfind_pdu<RawPDU>();
        auto payload = raw.payload();
        uint8_t *data = payload.data();
        uint32_t data_len = raw.payload_size();

        // Return vector<pair<fopd_packet_type_t, uint8_t *> 
        std::vector<std::pair<fopd_packet_type_t, std::vector<uint8_t>>> fopd_pkts = getPacketsFromRawTCP(data, data_len);

        // For each packet, initialize the right type then print it
        // std::cout << "Extracted " << fopd_pkts.size() << " packets from payload" << std::endl;

        for (size_t i = 0; i < fopd_pkts.size(); i++) {
            switch (fopd_pkts[i].first) {
                case FOPD_ENTITY_CLICK_PACKET:
                {
                    FiestaOnlinePacketEntityStats entity_pkt(fopd_pkts[i].second);
                    std::cout << entity_pkt << std::endl;
                }
                break;

                case FOPD_DAMAGE_PACKET:
                {
                    FiestaOnlinePacketDamage damage_pkt(fopd_pkts[i].second);
                    std::cout << damage_pkt << std::endl;
                }
                break;

                default:
                {
                    // std::cout << "Unknown packet" << std::endl;
                }
            }
        }
    }
    catch (pdu_not_found error) {
        // std::cerr << "[ERROR] No RawPDU found in packet" << std::endl;
        return false;
    }


    return true;
}

int main() {
    std::cout << "Starting ... " << std::endl;

    IPv4Address server_address(FIESTA_ONLINE_SERVER_ADDRESS);
    std::cout << "Server IP (default): " << server_address << std::endl;

    // Get network interface to tap
    NetworkInterface iface = NetworkInterface::default_interface();
    std::cout << "Interface name: " << iface.friendly_name().c_str() << std::endl;

    // Configure sniffer
    SnifferConfiguration config;
    config.set_promisc_mode(true);
    // Only inbound packets
    config.set_direction(PCAP_D_IN);
    // Only packets from server
    config.set_filter("ip src " + FIESTA_ONLINE_SERVER_ADDRESS);
    config.set_snap_len(65535);

    Sniffer sniffer(iface.name(), config);

    while(1) {
        sniffer.sniff_loop(sniffer_callback);
    }
    std::cout << "Done." << std::endl;
    return 0;
}
