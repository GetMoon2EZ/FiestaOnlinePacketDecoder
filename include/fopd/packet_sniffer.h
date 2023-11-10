#ifndef __FOPD_PACKET_SNIFFER_H__
#define __FOPD_PACKET_SNIFFER_H__

/**
 * @brief Start up the packet sniffing thread
 */
void sniffer_thread(void);

#include <stdint.h>

void test_packet_split(uint8_t *data, uint32_t data_len);

#endif // __FOPD_PACKET_SNIFFER_H__
