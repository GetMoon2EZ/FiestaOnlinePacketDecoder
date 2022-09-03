#include <iostream>
#include <iomanip>
#include <thread>

#include <utility>
#include <vector>
#include <queue>

#include <tins/tins.h>

#include "fopd/dps_meter.h"
#include "fopd/packet_sniffer.h"
#include "fopd/fopd_types.h"
#include "fopd/fopd_utils.h"
#include "fopd/fopd_packet.h"
#include "fopd/fopd_packet_entity_stats.h"
#include "fopd/fopd_packet_damage.h"
#include "fopd/gui.h"
#include "fopd/ping.h"


using namespace std;

int main() {
    cout << "Starting ... " << endl;

    const uint32_t DPS_UPDATE_TIME_MS = 100;
    const uint32_t PING_UPDATE_TIME_MS = 1000;

    fopd_damage_queue dmg_q;

    // Start and detach threads to avoid a call to terminate

    thread t1(dps_thread, &dmg_q, DPS_UPDATE_TIME_MS);
    t1.detach();

    thread t2(sniffer_thread, &dmg_q);
    t2.detach();

    thread t3(ping_thread, PING_UPDATE_TIME_MS);
    t3.detach();
    
    // Run the UI in the main thread
    run_gui();

    // Exit whenever the GUI returns (ie: window is closed)
    return 0;
}
