#include "fopd/fopd_packet.h"

#include <iostream>
#include <cstdint>
#include <cstring>

#include "fopd/fopd_consts.h"
#include "fopd/fopd_types.h"

uint8_t FiestaOnlinePacket::getPayloadLen(void) const
{
    return payload_len;
}

const uint8_t* FiestaOnlinePacket::getHeader(void) const
{
    return header;
}

const uint8_t* FiestaOnlinePacket::getPayload(void) const
{
    return payload;
}
