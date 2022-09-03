#ifndef __FOPD_PING_H__
#define __FOPD_PING_H__

#include <cstdint>

/**
 * @brief Start the ping thread
 * This thread pings Fiesta Online's server at regular intervals
 * 
 * @param update_delta_ms Delay between two ping updates
 */
void ping_thread(uint32_t update_delta_ms);

#endif // __FOPD_PING_H__