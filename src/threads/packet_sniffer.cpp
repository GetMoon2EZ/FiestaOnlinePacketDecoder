#include "fopd/packet_sniffer.h"

#include <stdio.h>
#include <map>
#include <tins/tins.h>

#include "fopd/fopd_consts.h"
#include "fopd/fopd_packet.h"
#include "fopd/fopd_data.h"
#include "fopd/network_stash.h"

using namespace Tins;

static Sniffer configure_sniffer(size_t server);
static bool process_packet(PDU& pkt);
static void process_stashed_data(network_stash *stash);
static void process_payloads(network_stash *stash, const uint8_t *data, uint32_t data_len);
static network_stash *get_port_stash(uint16_t port);

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
    snprintf(filter, sizeof(filter), "ip src %s and tcp", fo_servers[server].address);
    config.set_filter(filter);

    // Only inbound packets
    config.set_direction(PCAP_D_IN);
    config.set_snap_len(65535);
    config.set_timeout(1000);
    config.set_immediate_mode(true);

    Sniffer sniffer = Sniffer(iface.name(), config);
    /* Use pcap_dispatch so we can timeout if sniffing on the wrong address */
    sniffer.set_pcap_sniffing_method(pcap_dispatch);

    return sniffer;
}

static void
process_payloads(network_stash *stash, const uint8_t *data, uint32_t data_len)
{
    uint32_t current_pos = 0;
    uint32_t payload_len = 0;
    struct fopacket packet;
    int ret;

    while ((current_pos < data_len) && (get_payload_len(&data[current_pos], data_len - current_pos, &payload_len) != 1)) {
        // printf("payload_len = %u\n", payload_len);
        if (payload_len > (data_len - current_pos)) {
            /* Wait for next fragment */
            if (stash_initialize(stash, payload_len) != 0 || stash_push(stash, &data[current_pos], data_len - current_pos) != 0) {
                stash_clear(stash);
            }
            break;
        }

        if (payload_len == 0) {
            fprintf(stderr, "[ERROR] Empty payload\n");
            break;
        }

        // printf("New payload (%u): ", payload_len);
        // for (uint32_t i = 0; i < payload_len; i++) {
        //     printf("%02X ", data[current_pos + i]);
        // }
        // printf("\n");

        ret = parse_packet(&data[current_pos], payload_len, &packet);
        current_pos += payload_len;
        if (ret != 0) {
            fprintf(stderr, "[ERROR] Fail to parse packet\n");
            continue;
        }

        switch (packet.type) {
            case FOPACKET_DMG_AA:
            case FOPACKET_DMG_SPELL:
                handle_damage(&packet);
                break;
            case FOPACKET_ENTITY_INFO:
                handle_entity_info(&packet);
                break;
            case FOPACKET_PLAYER_INIT:
                handle_player_init(&packet);
                break;
            case FOPACKET_ASSIGN_ID:
                handle_assign_id(&packet);
                break;
            case FOPACKET_FRIEND_FIND:
                handle_friend_find(&packet);
                break;
            default:
                /* Nothing to do */
                // print_packet(&packet);
                break;
        }
    }
}

static bool process_packet(PDU& pkt)
{
    TCP *pkt_tcp;
    RawPDU *pkt_raw;
    RawPDU::payload_type payload;
    uint8_t *data;
    uint32_t data_len;
    network_stash *stash;

    /* Extract the TCP data from the packet */
    pkt_tcp = pkt.find_pdu<TCP>();
    if (pkt_tcp == 0) {
        return false;
    }

    pkt_raw = pkt_tcp->find_pdu<RawPDU>();
    if (pkt_raw == 0) {
        return false;
    }

    data_len = pkt_raw->payload_size();
    payload = pkt_raw->payload();
    data = payload.data();

    /* Debug ... */
    // printf("New packet (%u): ", data_len);
    // for (uint32_t i = 0; i < data_len; i++) {
    //     printf("%02X ", data[i]);
    // }
    // printf("\n");

    /* Find the stash for this destination port */
    stash = get_port_stash(pkt_tcp->dport());
    if (stash == NULL) {
        return false;
    }

    /* Process the data */
    if (is_stash_initialized(stash)) {
        /* Collect more data for the stash */
        uint32_t to_stash = (std::min)(data_len, stash_remaining_space(stash));
        stash_push(stash, data, to_stash);

        if (is_stash_full(stash)) {
            process_stashed_data(stash);
        }

        /* If we have not processed the full data */
        if (to_stash != data_len) {
            /* Offset the data for processing */
            data += to_stash;
            data_len -= to_stash;
        } else {
            /* Nothing more to process */
            return true;
        }
    }

    process_payloads(stash, data, data_len);
    return true;
}

static network_stash*
get_port_stash(uint16_t port)
{
    static std::map<uint16_t, network_stash*> port_stash;

    if (port_stash.find(port) == port_stash.end()) {
        printf("[PORT] %u\n", port);
        port_stash[port] = stash_create();
    }

    return port_stash[port];
}

static void
process_stashed_data(network_stash *stash)
{
    uint8_t *buf;
    uint32_t buf_size;

    if (stash_get_data(stash, &buf, &buf_size) != 0) {
        return;
    }

    process_payloads(stash, buf, buf_size);
    free(buf);
    stash_clear(stash);
}
