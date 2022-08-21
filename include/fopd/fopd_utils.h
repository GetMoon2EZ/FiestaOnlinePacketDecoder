#ifndef __FOPD_UTILS_H__
#define __FOPD_UTILS_H__

#include <iostream>

#include <utility>
#include <vector>

#include <fopd/fopd_consts.h>
#include <fopd/fopd_types.h>

/**
 * @brief Convert from little endian byte array (of 4 bytes) to uint32_t
 * 
 * @param byte_array 
 * @return uint32_t 
 */
uint32_t little_endian_byte_array_to_uint32(uint8_t *byte_array);

/**
 * @brief Get the corresponding packet type from a header
 * 
 * @param header Header byte array
 * @return fopd_packet_type_t
 */
fopd_packet_type_t packetTypeFromHeader(uint8_t header[FOPD_PACKET_HEADER_LEN]);

fopd_packet_type_t packetTypeFromData(uint8_t *data);

std::vector<std::pair<fopd_packet_type_t, std::vector<uint8_t>>> getPacketsFromRawTCP(uint8_t *tcp_data, uint32_t tcp_data_len);

#endif // __FOPD_UTILS_H__