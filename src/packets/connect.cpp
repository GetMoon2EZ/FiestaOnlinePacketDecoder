#include "fopd/fopd_packet_connect.h"

#include <string.h>

#include "fopd/fopd_consts.h"
#include "fopd/fopd_types.h"

#define FOPD_FRIEND_CONNECT_PACKET_MIN_LENGTH   20
#define FRIEND_CONNECT_CHAR_NAME_OFFSET         0

fopd_status_t FiestaOnlinePacketConnect::parsePayload(void)
{
    if (this->payload_len < FOPD_FRIEND_CONNECT_PACKET_MIN_LENGTH) {
        return FOPD_ERROR;
    }

    this->connect = this->header[0] == FOPD_FRIEND_CONNECT_PACKET_HEADER_B0;

    memcpy(this->char_name, &this->payload[FRIEND_CONNECT_CHAR_NAME_OFFSET], CHARACTER_NAME_LENGTH);
    return FOPD_OK;
}

const char *FiestaOnlinePacketConnect::getCharName(void) const
{
    return this->char_name;
}

const bool FiestaOnlinePacketConnect::getConnect(void) const
{
    return this->connect;
}
