#ifndef __FOPD_DPS_METER_H__
#define __FOPD_DPS_METER_H__

#include <cstdint>
#include <queue>
#include <chrono>
#include <mutex>
#include <utility>

#include "fopd/fopd_packet_damage.h"
#include "fopd/fopd_types.h"

typedef struct {
    std::queue<FiestaOnlinePacketDamage> q;
    std::mutex lock;
} fopd_damage_queue;

/**
 * @brief Start up the DPS calculating thread
 *
 * @param[in] q Queue that contains damage packets
 * @param[in] update_time_ms Determine the delay between two DPS updates 
 */
void dps_thread(fopd_damage_queue *dmg_q, const uint32_t update_time_ms);

#endif // __FOPD_DPS_METER_H__