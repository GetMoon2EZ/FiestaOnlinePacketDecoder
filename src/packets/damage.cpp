#include "fopd/fopd_packet_damage.h"

#include <cstdint>
#include <iostream>

#include "fopd/fopd_types.h"
#include "fopd/fopd_consts.h"


#define MAX_POSSIBLE_DAMAGE     1000000000ULL // 1 Billion

using namespace std;

fopd_status_t FiestaOnlinePacketDamage::parsePayload(void)
{
    switch (this->header[0]) {
        case FOPD_AA_DAMAGE_PACKET_HEADER_B0:
        {
            if (this->payload_len < FOPD_AA_DAMAGE_PACKET_PAYLOAD_MIN_LEN) {
                return FOPD_ERROR;
            }
            this->source_id = little_endian_byte_array_to_uint16(this->payload + AA_DAMAGE_SOURCE_ID_OFFSET);
            this->target_id = little_endian_byte_array_to_uint16(this->payload + AA_DAMAGE_TARGET_ID_OFFSET);
            this->spell = false;
            this->damage_value = little_endian_byte_array_to_uint32(this->payload + AA_DAMAGE_VALUE_OFFSET);
            this->target_remaining_health = little_endian_byte_array_to_uint32(this->payload + AA_TARGET_REMAINING_HEALTH_OFFSET);
        }
        break;

        case FOPD_SPELL_DAMAGE_PACKET_HEADER_B0:
        {
            if (this->payload_len < FOPD_SPELL_DAMAGE_PACKET_PAYLOAD_MIN_LEN) {
                return FOPD_ERROR;
            }
            this->source_id = little_endian_byte_array_to_uint16(this->payload + SPELL_DAMAGE_SOURCE_ID_OFFSET);
            this->target_id = little_endian_byte_array_to_uint16(this->payload + SPELL_DAMAGE_TARGET_ID_OFFSET);
            this->spell = true;
            this->damage_value = little_endian_byte_array_to_uint32(this->payload + SPELL_DAMAGE_VALUE_OFFSET);
            this->target_remaining_health = little_endian_byte_array_to_uint32(this->payload + SPELL_TARGET_REMAINING_HEALTH_OFFSET);
        }
        break;

        default:
            return FOPD_ERROR;
    }

    if (this->damage_value >= MAX_POSSIBLE_DAMAGE) {
        // Fix "infinite" damage bug
        this->damage_value = 0;
        return FOPD_ERROR;
    }

    return FOPD_OK;
}

uint16_t FiestaOnlinePacketDamage::getSourceID(void) const
{
    return this->source_id;
}

uint16_t FiestaOnlinePacketDamage::getTargetID(void) const
{
    return this->target_id;
}

bool FiestaOnlinePacketDamage::isSpell(void) const
{
    return this->spell;
}

uint32_t FiestaOnlinePacketDamage::getDamageValue(void) const
{
    return this->damage_value;
}

uint32_t FiestaOnlinePacketDamage::getTargetRemainingHealth(void) const
{
    return this->target_remaining_health;
}

ostream& operator<<(ostream& out, FiestaOnlinePacketDamage& pkt)
{
    out << "Damage packet:" << endl;
    out << "\t- Damage value: " << pkt.getDamageValue() << endl;
    out << "\t- Target remaining health: " << pkt.getTargetRemainingHealth() << " HP";
    return out;
}
