#ifndef __FOPD_PACKET_ENTITY_STATS_H__
#define __FOPD_PACKET_ENTITY_STATS_H__

#include <cstdint>
#include <vector>

#include "fopd/fopd_packet.h"


#define FOPD_ENTITY_CLICK_PACKET_PAYLOAD_MIN_LEN    (32 - FOPD_PACKET_HEADER_LEN)

// Current health info
#define ENTITY_CLICK_CURRENT_HEALTH_OFFSET  (5 - FOPD_PACKET_HEADER_LEN)
#define ENTITY_CLICK_CURRENT_HEALTH_LENGTH  4

// Max health info
#define ENTITY_CLICK_MAX_HEALTH_OFFSET      (9 - FOPD_PACKET_HEADER_LEN)
#define ENTITY_CLICK_MAX_HEALTH_LENGTH      4

// Current mana info
#define ENTITY_CLICK_CURRENT_MANA_OFFSET    (13 - FOPD_PACKET_HEADER_LEN)
#define ENTITY_CLICK_CURRENT_MANA_LENGTH    4

// Max mana info
#define ENTITY_CLICK_MAX_MANA_OFFSET        (17 - FOPD_PACKET_HEADER_LEN)
#define ENTITY_CLICK_MAX_MANA_LENGTH        4

// Level info
#define ENTITY_CLICK_LEVEL_OFFSET           (29 - FOPD_PACKET_HEADER_LEN)
#define ENTITY_CLICK_LEVEL_LENGTH           1

class FiestaOnlinePacketEntityStats: public FiestaOnlinePacket
{
private:
    uint32_t current_health = 0;
    uint32_t max_health = 0;
    uint32_t current_mana = 0;
    uint32_t max_mana = 0;
    uint8_t level = 0;

    fopd_status_t parsePayload(void);

public:
    FiestaOnlinePacketEntityStats(void) { }

    FiestaOnlinePacketEntityStats(uint8_t* data)
    :FiestaOnlinePacket(data) {
        parsePayload();
    }

    FiestaOnlinePacketEntityStats(std::vector<uint8_t> data)
    :FiestaOnlinePacket(data) {
        parsePayload();
    }

    uint32_t getCurrentHealth(void) const;
    uint32_t getMaxHealth(void) const;
    uint32_t getCurrentMana(void) const;
    uint32_t getMaxMana(void) const;
    uint8_t getLevel(void) const;

    friend std::ostream& operator<<(std::ostream& out, FiestaOnlinePacketEntityStats &pkt);
};

#endif // __FOPD_PACKET_ENTITY_STATS_H__