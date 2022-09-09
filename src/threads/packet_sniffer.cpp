#include "fopd/packet_sniffer.h"

#include <chrono>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include <tins/tins.h>

#include "fopd/fopd_consts.h"
#include "fopd/fopd_packet.h"
#include "fopd/fopd_packet_damage.h"
#include "fopd/fopd_packet_entity_stats.h"
#include "fopd/dps_meter.h"

using namespace Tins;

bool process_packet(PDU& pkt, fopd_damage_queue *dmg_q)
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
                    // std::cout << entity_pkt << std::endl;
                }
                break;

                case FOPD_DAMAGE_PACKET:
                {
                    FiestaOnlinePacketDamage damage_pkt(fopd_pkts[i].second);
                    std::lock_guard<std::mutex> lk(dmg_q->lock);
                    dmg_q->q.push(damage_pkt);
                    // std::cout << damage_pkt << std::endl;
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
        return false;
    }
    return true;
}

void sniffer_thread(fopd_damage_queue *dmg_q)
{
    std::cout << "Server IP (default): " << FIESTA_ONLINE_SERVER_ADDRESS << std::endl;

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
        sniffer.sniff_loop(std::bind(&process_packet, std::placeholders::_1, dmg_q));
    }
}