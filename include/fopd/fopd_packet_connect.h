#ifndef __FOPD_PACKET_CONNECT_H__
#define __FOPD_PACKET_CONNECT_H__

#include "fopd/fopd_packet.h"

#include <fopd/fopd_types.h>
#include <fopd/fopd_consts.h>

#define CHARACTER_NAME_LENGTH  16

class FiestaOnlinePacketConnect: public FiestaOnlinePacket
{
private:
    char char_name[CHARACTER_NAME_LENGTH] = { 0 };
    bool connect = false;

    fopd_status_t parsePayload(void);

public:
    FiestaOnlinePacketConnect(uint8_t *data)
    :FiestaOnlinePacket(data) {
        parsePayload();
    }

    FiestaOnlinePacketConnect(std::vector<uint8_t> data)
    :FiestaOnlinePacket(data) {
        parsePayload();
    }

    const char *FiestaOnlinePacketConnect::getCharName(void) const;
    const bool FiestaOnlinePacketConnect::getConnect(void) const;
};

#endif // __FOPD_PACKET_CONNECT_H__
