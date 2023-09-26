#ifndef __FOPD_PACKET_ASSIGN_ID_H__
#define __FOPD_PACKET_ASSIGN_ID_H__

#include "fopd/fopd_packet.h"

#include <stdint.h>

#include <fopd/fopd_types.h>

class FiestaOnlinePacketAssignID: public FiestaOnlinePacket
{
private:
    uint16_t id = 0;

    fopd_status_t parsePayload(void);

public:
    FiestaOnlinePacketAssignID(uint8_t *data)
    :FiestaOnlinePacket(data) {
        parsePayload();
    }

    FiestaOnlinePacketAssignID(std::vector<uint8_t> data)
    :FiestaOnlinePacket(data) {
        parsePayload();
    }

    uint16_t FiestaOnlinePacketAssignID::getID(void) const;
};

#endif // __FOPD_PACKET_ASSIGN_ID_H__
