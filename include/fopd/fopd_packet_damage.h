#ifndef __FOPD_PACKET_DAMAGE_H__
#define __FOPD_PACKET_DAMAGE_H__

#include "fopd/fopd_packet.h"

#include <cstdint>
#include <iostream>
#include <vector>
#include <stdbool.h>

#include <fopd/fopd_types.h>
#include <fopd/fopd_consts.h>


/*
 * Spell damage packet structure:
 *     - Packet length (1 byte)
 *     - Packet header (2 bytes: 0x52 0x24)
 *     - Unknown data (2 bytes)
 *     - Source ID (2 bytes)
 *     - Unknown data (5 bytes)
 *     - Target ID (2 bytes)
 *     - Unknown data (1 byte)
 *     - Killing blow flag (1 byte)
 *     - Damage done (4 bytes)
 *     - Target remaining HP (4 bytes)
 *     - Unknown data (2 bytes)
 *
 * Suspected information in the unknown sections:
 *     - "Critical hit" flag
 *     - "Miss" flag
 *     - "Block" flag
 *     - Spell ID
 *     - Target ID
 *     - Source ID (player/mob ID)
 */

#define FOPD_SPELL_DAMAGE_PACKET_PAYLOAD_MIN_LEN    (25 - FOPD_PACKET_HEADER_LEN)

#define SPELL_DAMAGE_SOURCE_ID_OFFSET                  2
#define SPELL_DAMAGE_SOURCE_ID_LEN                     2

#define SPELL_DAMAGE_TARGET_ID_OFFSET                  9
#define SPELL_DAMAGE_TARGET_ID_LEN                     2

// Spell damage value
#define SPELL_DAMAGE_VALUE_OFFSET                   13
#define SPELL_DAMAGE_VALUE_LEN                      4

// Remaining health
#define SPELL_TARGET_REMAINING_HEALTH_OFFSET        (SPELL_DAMAGE_VALUE_OFFSET + SPELL_DAMAGE_VALUE_LEN)
#define SPELL_TARGET_REMAINING_HEALTH_LEN           4

/*
 * Auto-attack damage packet structure:
 *     - Packet length (1 byte)
 *     - Packet header (2 bytes: 0x52 0x24)
 *     - Source ID (2 bytes)
 *     - Target ID (2 bytes)
 *     - Unknown data (2 byte)
 *     - Damage done (4 bytes)
 *     - Target remaining HP (4 bytes)
 *     - Unknown data (4 bytes): probably counters
 *
 * Suspected information in the unknown sections:
 *     - "Miss" flag
 *     - "Block" flag
 *     - Target ID
 *     - Source ID (player/mob ID)
 */

#define FOPD_AA_DAMAGE_PACKET_PAYLOAD_MIN_LEN       (20 - FOPD_PACKET_HEADER_LEN)

#define AA_DAMAGE_SOURCE_ID_OFFSET                  0
#define AA_DAMAGE_SOURCE_ID_LEN                     2

#define AA_DAMAGE_TARGET_ID_OFFSET                  (AA_DAMAGE_SOURCE_ID_OFFSET + AA_DAMAGE_SOURCE_ID_LEN)
#define AA_DAMAGE_TARGET_ID_LEN                     2

// Auto-attack damage value
#define AA_DAMAGE_VALUE_OFFSET                      6
#define AA_DAMAGE_VALUE_LEN                         4

// Remaining health
#define AA_TARGET_REMAINING_HEALTH_OFFSET           (AA_DAMAGE_VALUE_OFFSET + AA_DAMAGE_VALUE_LEN)
#define AA_TARGET_REMAINING_HEALTH_LEN              4

class FiestaOnlinePacketDamage: public FiestaOnlinePacket
{
private:
    uint16_t source_id = 0;
    uint16_t target_id = 0;
    bool spell = false;
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

    uint16_t getSourceID(void) const;
    uint16_t getTargetID(void) const;
    bool isSpell(void) const;
    uint32_t getDamageValue(void) const;
    uint32_t getTargetRemainingHealth(void) const;

    friend std::ostream& operator<<(std::ostream& out, FiestaOnlinePacketDamage& pkt);
};

#endif // __FOPD_PACKET_DAMAGE_H__