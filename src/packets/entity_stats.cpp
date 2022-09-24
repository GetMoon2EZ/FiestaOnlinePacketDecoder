#include "fopd/fopd_packet_entity_stats.h"

#include <iostream>

#include "fopd/fopd_utils.h"
#include "fopd/fopd_types.h"


fopd_status_t FiestaOnlinePacketEntityStats::parsePayload(void) {
    // Debug
    // for (int i = 0; i < payload_len; i++) {
    //     std::cout << std::hex << static_cast<int>(payload[i]) << " "; 
    // }
    // std::cout << std::dec << std::endl;

    if (this->payload_len < FOPD_ENTITY_CLICK_PACKET_PAYLOAD_MIN_LEN) {
        // std::cerr << "[ERROR] Cannot parse given data" << std::endl;
        return FOPD_ERROR;
    }
    // Parse current health
    this->current_health = little_endian_byte_array_to_uint32(this->payload + ENTITY_CLICK_CURRENT_HEALTH_OFFSET);
    this->max_health = little_endian_byte_array_to_uint32(this->payload + ENTITY_CLICK_MAX_HEALTH_OFFSET);
    this->current_mana = little_endian_byte_array_to_uint32(this->payload + ENTITY_CLICK_CURRENT_MANA_OFFSET);
    this->max_mana = little_endian_byte_array_to_uint32(this->payload + ENTITY_CLICK_MAX_MANA_OFFSET);
    this->level = this->payload[ENTITY_CLICK_LEVEL_OFFSET];
    return FOPD_OK;
}

uint32_t FiestaOnlinePacketEntityStats::getCurrentHealth(void) const
{
    return this->current_health;
}

uint32_t FiestaOnlinePacketEntityStats::getMaxHealth(void) const
{
    return this->max_health;
}

uint32_t FiestaOnlinePacketEntityStats::getCurrentMana(void) const
{
    return this->current_mana;
}

uint32_t FiestaOnlinePacketEntityStats::getMaxMana(void) const
{
    return this->max_mana;
}

uint8_t FiestaOnlinePacketEntityStats::getLevel(void) const
{
    return this->level;
}

std::ostream& operator<<(std::ostream& out, FiestaOnlinePacketEntityStats &pkt)
{
    out << "Entity stats:" << std::endl;
    out << "\t- Level:" << static_cast<int>(pkt.getLevel()) << std::endl;
    out << "\t- Current health: " << pkt.getCurrentHealth() << std::endl;
    out << "\t- Max health: " << pkt.getMaxHealth() << std::endl;
    out << "\t- Current mana: " << pkt.getCurrentMana() << std::endl;
    out << "\t- Max mana: " << pkt.getMaxMana();
    return out;
}