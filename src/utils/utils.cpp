#include "fopd/fopd_utils.h"

#include <cstdint>
#include <cstdlib>

#include <vector>

#include "fopd/fopd_types.h"
#include "fopd/fopd_consts.h"

using namespace std;

uint32_t little_endian_byte_array_to_uint32(uint8_t *byte_array)
{
    uint32_t ret = 0;
    for (int i = 0; i < 4; i++) {
        ret = ret << 8;
        ret += byte_array[3 - i];
    }

    return ret;
}

uint16_t little_endian_byte_array_to_uint16(uint8_t *byte_array)
{
    uint16_t ret = 0;
    for (int i = 0; i < 2; i++) {
        ret = ret << 8;
        ret += byte_array[1 - i];
    }

    return ret;
}

fopd_packet_type_t packetTypeFromHeader(uint8_t header[FOPD_PACKET_HEADER_LEN])
{
    if (memcmp(header, FOPD_SPELL_DAMAGE_PACKET_HEADER, FOPD_PACKET_HEADER_LEN) == 0 || memcmp(header, FOPD_AA_DAMAGE_PACKET_HEADER, FOPD_PACKET_HEADER_LEN) == 0) {
        return FOPD_DAMAGE_PACKET;
    }
    else if (memcmp(header, FOPD_ENTITY_CLICK_PACKET_HEADER, FOPD_PACKET_HEADER_LEN) == 0) {
        return FOPD_ENTITY_CLICK_PACKET;
    }
    else {
        return FOPD_UNKNOWN_PACKET;
    }
}

fopd_packet_type_t packetTypeFromData(uint8_t *data)
{
    uint8_t header[FOPD_PACKET_HEADER_LEN] = { 0 };
    memcpy(header, &data[FOPD_PACKET_HEADER_OFFSET], FOPD_PACKET_HEADER_LEN);
    return packetTypeFromHeader(header);
}

vector<pair<fopd_packet_type_t, vector<uint8_t>>> getPacketsFromRawTCP(uint8_t *tcp_data, uint32_t tcp_data_len)
{
    if (tcp_data == NULL)
        return vector<pair<fopd_packet_type_t, vector<uint8_t>>> ();

    std::vector<std::pair<fopd_packet_type_t, std::vector<uint8_t>>> ret;
    uint32_t pos = 0;

    while (pos < tcp_data_len) {
        uint16_t payload_len = ((uint16_t) tcp_data[pos + FOPD_PACKET_PAYLOAD_LEN_OFFSET]) + 1;

        if (payload_len == 0 || pos + payload_len > tcp_data_len) {
            break;
        }

        std::vector<uint8_t> payload(&tcp_data[pos], &tcp_data[pos + payload_len]);

        // Get packet type
        fopd_packet_type_t type = packetTypeFromData(&tcp_data[pos]);

        ret.push_back(pair<fopd_packet_type_t, vector<uint8_t>>(type, payload));

        // Go to the next packet
        pos += payload_len;
    }

    return ret;
}

char *vec_u8_to_hex_str(std::vector<uint8_t> v, size_t *s)
{
    char *ret = NULL;
    char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    *s = v.size() * 2 + 1;
    ret = (char *) calloc(*s, sizeof(uint8_t));
    for (size_t i = 0; i < v.size(); i++) {
        ret[i * 2] = hex[v[i] >> 4];
        ret[i * 2 +  1] = hex[v[i] & 0x0F];
    }
    ret[*s-1] = '\0';

    return ret;
}