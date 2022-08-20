#ifndef __FOPD_PACKETS_H__
#define __FOPD_PACKETS_H__

#include <iostream>
#include <chrono>
#include <vector>
#include <cassert>

#include "fopd/fopd_types.h"
#include "fopd/fopd_consts.h"
#include "fopd/fopd_utils.h"

/**
 * @brief Generic class for packets
 * 
 */
class FiestaOnlinePacket
{
protected:
    // Timestamp
    std::chrono::high_resolution_clock::time_point timestamp;
    // Indicates the length of the payload
    uint8_t payload_len = 0;
    // Array of the header
    uint8_t header[FOPD_PACKET_HEADER_LEN]= { 0 };
    // Packet payload (TCP Data excluding payload_len and header fields)
    uint8_t payload[FOPD_PACKET_PAYLOAD_MAX_LEN] = { 0 };

public:
    FiestaOnlinePacket(void) {
        this->timestamp = std::chrono::high_resolution_clock::now();
    }

    FiestaOnlinePacket(uint8_t *data) {
        // Set the timestamp
        this->timestamp = std::chrono::high_resolution_clock::now();
        // Get payload length from data
        this->payload_len = data[FOPD_PACKET_PAYLOAD_LEN_OFFSET];
        // Payload length includes the header, need to substract header len
        assert(this->payload_len >= FOPD_PACKET_HEADER_LEN);
        this->payload_len -= FOPD_PACKET_HEADER_LEN;
        // Set header
        memcpy(this->header, &data[FOPD_PACKET_HEADER_OFFSET], FOPD_PACKET_HEADER_LEN);
        // Copy remaining data as payload
        memcpy(this->payload, &data[FOPD_PACKET_PAYLOAD_OFFSET], this->payload_len);
    }

    FiestaOnlinePacket(std::vector<uint8_t> data) {
        // Set the timestamp
        this->timestamp = std::chrono::high_resolution_clock::now();

        uint8_t *_data = &data[0];
        this->payload_len = _data[FOPD_PACKET_PAYLOAD_LEN_OFFSET];
        // Payload length includes the header, need to substract header len
        assert(this->payload_len >= FOPD_PACKET_HEADER_LEN);
        this->payload_len -= FOPD_PACKET_HEADER_LEN;
        // Set header
        memcpy(this->header, &_data[FOPD_PACKET_HEADER_OFFSET], FOPD_PACKET_HEADER_LEN);
        // Copy remaining data as payload
        memcpy(this->payload, &_data[FOPD_PACKET_PAYLOAD_OFFSET], this->payload_len);
    }

    FiestaOnlinePacket(fopd_packet_type_t packet_type) {
        this->timestamp = std::chrono::high_resolution_clock::now();
        switch (packet_type) {
            case FOPD_DAMAGE_PACKET:
                memcpy(this->header, FOPD_DAMAGE_PACKET_HEADER, FOPD_PACKET_HEADER_LEN);
                break;
            case FOPD_ENTITY_CLICK_PACKET:
                memcpy(this->header, FOPD_ENTITY_CLICK_PACKET_HEADER, FOPD_PACKET_HEADER_LEN);
                break;
            default:
                // Unknown packet type, cannot set the header
                memset(this->header, 0, FOPD_PACKET_HEADER_LEN * sizeof(*(this->header)));
        }
    }

    uint8_t getPayloadLen(void) const;
    const uint8_t* getHeader(void) const;
    const uint8_t* getPayload(void) const;
    const std::chrono::high_resolution_clock::time_point getTimestamp(void) const;
};

#endif // __FOPD_PACKETS_H__