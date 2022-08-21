#ifndef __FOPD_PACKET_SNIFFER_H__
#define __FOPD_PACKET_SNIFFER_H__

#include <chrono>
#include <queue>
#include <utility>

#include "fopd/fopd_packet_damage.h"
#include "fopd/fopd_types.h"
#include "fopd/dps_meter.h"

/**
 * @brief Start up the packet sniffing thread
 * 
 * @param[in] q Queue that contains damage packets
 */
void sniffer_thread(fopd_damage_queue *dmg_q);

#endif // __FOPD_PACKET_SNIFFER_H__