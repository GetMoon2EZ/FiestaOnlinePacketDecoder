#include "fopd/packet_sniffer.h"

#include <stdio.h>
#include <tins/tins.h>

#include "fopd/fopd_consts.h"
#include "fopd/fopd_packet.h"
#include "fopd/fopd_data.h"

using namespace Tins;

static Sniffer configure_sniffer(size_t server);

/* Simple stashing mechanism to allow fragmented packet reconstruction */
static int stash_init(uint32_t total_len);
static void stash_clear(void);
static int stash_push(const uint8_t *buf, uint32_t buf_size);
static uint16_t stash_remaining_space(void);
static bool stash_full(void);
static bool stash_empty(void);

#define TCP_STASH_SIZE      65535

uint8_t tcp_stash[TCP_STASH_SIZE];
uint16_t stashed_len = 0;
uint16_t stash_expected_len = 0;


void
process_payloads(const uint8_t *data, uint32_t data_len)
{
    uint32_t current_pos = 0;
    uint32_t payload_len = 0;
    struct fopacket packet;
    int ret;

    while ((current_pos < data_len) && (get_payload_len(&data[current_pos], data_len - current_pos, &payload_len) != 1)) {
        if (payload_len > (data_len - current_pos)) {
            /* Wait for next fragment */
            if (stash_init(payload_len) != 0 || stash_push(&data[current_pos], data_len - current_pos) != 0) {
                stash_clear();
            }
            break;
        }

        if (payload_len == 0) {
            fprintf(stderr, "[ERROR] Empty payload\n");
            break;
        }

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

bool process_packet(PDU& pkt)
{
    try {
        // Extract raw TCP data from the packet
        const RawPDU &raw = pkt.rfind_pdu<RawPDU>();
        auto payload = raw.payload();
        uint8_t *data = payload.data();
        uint32_t data_len = raw.payload_size();

        // printf("New packet (%u): ", data_len);
        // for (uint32_t i = 0; i < data_len; i++) {
        //     printf("%02X ", data[i]);
        // }
        // printf("\n");

        uint32_t to_stash;

        if (!stash_empty()) {
            /* Collect more data for the stash */
            to_stash = (std::min)(data_len, (uint32_t) stash_remaining_space());
            stash_push(data, to_stash);

            if (stash_full()) {
                process_payloads(tcp_stash, (uint32_t) stashed_len);
                stash_clear();
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

        process_payloads(data, data_len);

    } catch (pdu_not_found error) {
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
    snprintf(filter, sizeof(filter), "ip src %s and tcp", fo_servers[server].address);
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

static int
stash_init(uint32_t total_len)
{
    stash_clear();

    if (total_len > TCP_STASH_SIZE) {
        fprintf(stderr, "[ERROR] stash_init failed: %d > %d\n", total_len, TCP_STASH_SIZE);
        return -1;
    }

    stash_expected_len = total_len;
    return 0;
}

static void
stash_clear(void)
{
    stashed_len = 0;
    stash_expected_len = 0;
    memset(tcp_stash, 0, TCP_STASH_SIZE);
}

static int
stash_push(const uint8_t *buf, uint32_t buf_size)
{
    if (stashed_len + buf_size > stash_expected_len) {
        /* Overflow */
        return -1;
    }

    memcpy(&tcp_stash[stashed_len], buf, buf_size);
    stashed_len += buf_size;

    return 0;
}

static uint16_t
stash_remaining_space(void)
{
    return stash_expected_len - stashed_len;
}


static bool
stash_full(void)
{
    return stash_expected_len == stashed_len;
}

static bool
stash_empty(void)
{
    return stash_expected_len == 0;
}
