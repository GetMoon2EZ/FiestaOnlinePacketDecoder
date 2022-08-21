#ifndef __FOPD_PACKET_DAMAGE_H__
#define __FOPD_PACKET_DAMAGE_H__

#include "fopd/fopd_packet.h"

#include <cstdint>
#include <iostream>

#include <vector>

#include <fopd/fopd_types.h>
#include <fopd/fopd_consts.h>


#define FOPD_SPELL_DAMAGE_PACKET_PAYLOAD_MIN_LEN    (25 - FOPD_PACKET_HEADER_LEN)

// Spell damage value
#define SPELL_DAMAGE_VALUE_OFFSET                   (15 - FOPD_PACKET_HEADER_LEN)
#define SPELL_DAMAGE_VALUE_LEN                      4

// Remaining health
#define SPELL_TARGET_REMAINING_HEALTH_OFFSET        (SPELL_DAMAGE_VALUE_OFFSET + SPELL_DAMAGE_VALUE_LEN)
#define SPELL_TARGET_REMAINING_HEALTH_LEN           4

#define FOPD_AA_DAMAGE_PACKET_PAYLOAD_MIN_LEN       (20 - FOPD_PACKET_HEADER_LEN)

// Auto-attack damage value
#define AA_DAMAGE_VALUE_OFFSET                      (8 - FOPD_PACKET_HEADER_LEN)
#define AA_DAMAGE_VALUE_LEN                         4

// Remaining health
#define AA_TARGET_REMAINING_HEALTH_OFFSET           (AA_DAMAGE_VALUE_OFFSET + AA_DAMAGE_VALUE_LEN)
#define AA_TARGET_REMAINING_HEALTH_LEN              4

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