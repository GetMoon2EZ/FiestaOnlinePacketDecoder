#ifndef __FOPD_UTILS_H__
#define __FOPD_UTILS_H__

#include <iostream>

#include <utility>
#include <vector>
#include <winsock2.h>

#include <fopd/fopd_consts.h>
#include <fopd/fopd_types.h>


#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

/**
 * @brief Comparator for maps using char* as keys
 */
struct cmp_str {
    bool operator()(char const *a, char const *b) const
    {
        return strcmp(a, b) < 0;
    }
};

/**
 * @brief Convert from little endian byte array (of 4 bytes) to uint32_t
 *
 * @param byte_array
 * @return uint32_t
 */
uint32_t little_endian_byte_array_to_uint32(uint8_t *byte_array);

/**
 * @brief Convert from little endian byte array (of 2 bytes) to uint16_t
 *
 * @param byte_array
 * @return uint16_t
 */
uint16_t little_endian_byte_array_to_uint16(uint8_t *byte_array);

/**
 * @brief Get the corresponding packet type from a header
 * 
 * @param header Header byte array
 * @return fopd_packet_type_t
 */
fopd_packet_type_t packetTypeFromHeader(uint8_t header[FOPD_PACKET_HEADER_LEN]);

fopd_packet_type_t packetTypeFromData(uint8_t *data);

std::vector<std::pair<fopd_packet_type_t, std::vector<uint8_t>>> getPacketsFromRawTCP(uint8_t *tcp_data, uint32_t tcp_data_len);

char *vec_u8_to_hex_str(std::vector<uint8_t> v, size_t *s);

/** Subtract the `struct timeval' values X and Y, storing the result in RESULT.
 * Return 1 if the difference is negative, otherwise 0.
 */
int timeval_subtract(struct timeval x, struct timeval y, struct timeval *result);

int gettimeofday(struct timeval * tp, struct timezone * tzp);

void print_system_error(const char *func_name);

/**
 * @brief Little endian to system short
 *
 * @param x Value to convert
 * @return uint16_t Converted value or x depending on system endianess
 */
uint16_t letoss(uint16_t x);

/**
 * @brief Little endian to system long
 *
 * @param x Value to convert
 * @return uint32_t Converted value or x depending on system endianess
 */
uint32_t letosl(uint32_t x);

#endif // __FOPD_UTILS_H__
