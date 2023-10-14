#ifndef __FO_PING_H__
#define __FO_PING_H__

#include <stdint.h>

#define PING_ERROR      99999

/**
 * @brief Ping the provided IPv4 address
 *
 * @return PING_ERROR on error, round trip time in ms otherwise.
 */
uint32_t ping(uint32_t ip_addr);

#endif // __FO_PING_H__
