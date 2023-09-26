#include "fopd/fopd_packet_assign_id.h"

#include <stdint.h>

#include "fopd/fopd_consts.h"
#include "fopd/fopd_types.h"
#include "fopd/fopd_utils.h"

#define FOPD_ASSIGN_ID_PACKET_MIN_LENGTH    3
#define ASSIGN_ID_PLAYER_ID_OFFSET          0

fopd_status_t FiestaOnlinePacketAssignID::parsePayload(void)
{
    if (this->payload_len < FOPD_ASSIGN_ID_PACKET_MIN_LENGTH) {
        printf("[ERROR] payload_len = %u\n", this->payload_len);
        return FOPD_ERROR;
    }

    this->id = little_endian_byte_array_to_uint16(this->payload + ASSIGN_ID_PLAYER_ID_OFFSET);

    return FOPD_OK;
}

uint16_t FiestaOnlinePacketAssignID::getID(void) const
{
    return this->id;
}
