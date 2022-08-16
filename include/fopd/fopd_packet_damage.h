#ifndef __FOPD_PACKET_DAMAGE_H__
#define __FOPD_PACKET_DAMAGE_H__

#include "fopd/fopd_packet.h"

#include <cstdint>
#include <iostream>

#include <vector>

#include <fopd/fopd_types.h>
#include <fopd/fopd_consts.h>


#define FOPD_DAMAGE_PACKET_PAYLOAD_MIN_LEN  (25 - FOPD_PACKET_HEADER_LEN)

// Damage value
#define DAMAGE_VALUE_OFFSET                 (15 - FOPD_PACKET_HEADER_LEN)
#define DAMAGE_VALUE_LEN                    4

// Remaining health
#define TARGET_REMAINING_HEALTH_OFFSET      (DAMAGE_VALUE_OFFSET + DAMAGE_VALUE_LEN)
#define TARGET_REMAINING_HEALTH_LEN      4

class FiestaOnlinePacketDamage: public FiestaOnlinePacket
{
private:
    uint32_t damage_value = 0;
    uint32_t target_remaining_health = 0;

    fopd_status_t parsePayload(void);

public:
    FiestaOnlinePacketDamage(uint8_t *data)
    :FiestaOnlinePacket(data) {
        parsePayload();
    }

    FiestaOnlinePacketDamage(std::vector<uint8_t> data)
    :FiestaOnlinePacket(data) {
        parsePayload();
    }

    uint32_t getDamageValue(void) const;
    uint32_t getTargetRemainingHealth(void) const;

    friend std::ostream& operator<<(std::ostream& out, FiestaOnlinePacketDamage& pkt);
};

#endif // __FOPD_PACKET_DAMAGE_H__