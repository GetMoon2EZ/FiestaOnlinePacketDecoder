#ifndef __FOPD_UTILS_CONSTS_H__
#define __FOPD_UTILS_CONSTS_H__

#include <cstdint>
#include <string>

// Field offsets in raw datastream
#ifndef FOPD_PACKET_PAYLOAD_LEN_OFFSET
    #define FOPD_PACKET_PAYLOAD_LEN_OFFSET  0
#endif

#ifndef FOPD_PACKET_HEADER_OFFSET
    #define FOPD_PACKET_HEADER_OFFSET       1
#endif

#ifndef FOPD_PACKET_PAYLOAD_OFFSET
    #define FOPD_PACKET_PAYLOAD_OFFSET      3
#endif

// Buffer lengths
#ifndef FOPD_PACKET_HEADER_LEN
    #define FOPD_PACKET_HEADER_LEN      2
#endif

#ifndef FOPD_PACKET_PAYLOAD_MAX_LEN
    #define FOPD_PACKET_PAYLOAD_MAX_LEN     255
#endif

extern const std::string FIESTA_ONLINE_SERVER_ADDRESS;
extern const uint8_t FOPD_DAMAGE_PACKET_HEADER[FOPD_PACKET_HEADER_LEN];
extern const uint8_t FOPD_ENTITY_CLICK_PACKET_HEADER[FOPD_PACKET_HEADER_LEN];

#endif // __FOPD_UTILS_CONSTS_H__