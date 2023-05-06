#include "fopd/dps_meter.h"

#include <cstdint>
#include <numeric>
#include <chrono>
#include <thread>

#include <queue>
#include <utility>

#include "fopd/fopd_packet_damage.h"
#include "fopd/fopd_data.h"


using namespace std;

static uint32_t get_dmg_queue_max(queue<FiestaOnlinePacketDamage> q)
{
    uint32_t max_dmg = 0;
    while (!q.empty()) {
        max_dmg = max(max_dmg, q.front().getDamageValue());
        q.pop();
    }
    return max_dmg;
}

static uint32_t get_dmg_queue_sum(queue<FiestaOnlinePacketDamage> q)
{
    // It is VERY unlikely that the DPS of one player exceeds INT_MAX
    uint32_t sum = 0;
    while (!q.empty()) {
        sum += q.front().getDamageValue();
        q.pop();
    }
    return sum;
}

static void print_dps(const int dps, const int max_dps)
{
    // Cast to int for readability
    cout << "Current player DPS:   " << dps << endl;
    cout << "Highest DPS recorder: " << max_dps << endl;
}

// queue is shared between producer thread and consumer thread
void dps_thread(fopd_damage_queue* const dmg_q, const uint32_t update_delta_ms)
{
    FOPDData *data = FOPDData::getInstance();
    uint32_t count = 0;
    // update_delta_ms should be less than 1 sec
    uint32_t reset_count = 1000 / update_delta_ms;
    while (true) {
        // Lock the mutex
        dmg_q->lock.lock();

        // Process current queue
        auto last_sec = chrono::high_resolution_clock::now() - chrono::seconds{ 1 };
        if (!dmg_q->q.empty()) {
            // Remove expired packets
            while (!dmg_q->q.empty() && dmg_q->q.front().getTimestamp() <= last_sec) {
                dmg_q->q.pop();
            }
            // The dps (damage mean) is computed from the start at each iteration
            // A faster approach would be to maintain the queue sum, subtract expired elements then add the new elements
        }

        // Update dps data
        data->setDPS(get_dmg_queue_sum(dmg_q->q));
        // Debug
        // print_dps(data->getDPS(), data->getMaxDPS());

        if (count++ == reset_count) {
            data->trySetMaxDmg(get_dmg_queue_max(dmg_q->q));
            count = 0;
        }
        // Queue ownership not needed anymore
        dmg_q->lock.unlock();

        // Pause current thread
        this_thread::sleep_for(chrono::milliseconds(update_delta_ms));
    }
}