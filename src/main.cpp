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


using namespace std;

int main() {
    cout << "Starting ... " << endl;

    const int DPS_UPDATE_TIME_MS = 1000;
    fopd_damage_queue dmg_q;

    thread t1(dps_thread, &dmg_q, DPS_UPDATE_TIME_MS);
    thread t2(sniffer_thread, &dmg_q);

    while (1) { }

    cout << "Done." << std::endl;
    return 0;
}
