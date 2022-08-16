#include "fopd/fopd_packet_damage.h"

#include <cstdint>
#include <iostream>

#include "fopd/fopd_types.h"


using namespace std;

fopd_status_t FiestaOnlinePacketDamage::parsePayload(void)
{
    if (this->payload_len < FOPD_DAMAGE_PACKET_PAYLOAD_MIN_LEN) {
        cerr << "[ERROR] Cannot parse given data" << endl;
        return FOPD_ERROR;
    }

    this->damage_value = little_endian_byte_array_to_uint32(this->payload + DAMAGE_VALUE_OFFSET);
    this->target_remaining_health = little_endian_byte_array_to_uint32(this->payload + TARGET_REMAINING_HEALTH_OFFSET);

    return FOPD_OK;
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
