#ifndef __FOPD_PACKET_DAMAGE_H__
#define __FOPD_PACKET_DAMAGE_H__

#include <cstdint>

#include "fopd/fopd_packet.h"

class FiestaOnlinePacketDamage: public FiestaOnlinePacket
{
private:
public:
    FiestaOnlinePacketDamage(uint8_t *data) {
        // TODO
    }
};

#endif // __FOPD_PACKET_DAMAGE_H__