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
#include "fopd/fopd_data.h"

using namespace Tins;

static Sniffer configure_sniffer(size_t server);

bool process_packet(PDU& pkt, fopd_damage_queue *dmg_q)
{
    FOPDData *fopd_data = FOPDData::getInstance();
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
                    std::cout << "Target remaining health : " << static_cast<int>(damage_pkt.getTargetRemainingHealth()) << std::endl;
                    fopd_data->setTargetRemainingHealth(damage_pkt.getTargetRemainingHealth());
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
        sniffer.sniff_loop(std::bind(&process_packet, std::placeholders::_1, dmg_q), 1);
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
