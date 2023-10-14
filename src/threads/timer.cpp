#include "fopd/fo_timer.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <winuser.h>

#include "fopd/fo_ping.h"
#include "fopd/fopd_data.h"
#include "fopd/fopd_consts.h"
#include "fopd/fopd_utils.h"


#define TIMER_NAME_LEN  16

typedef enum {
    TIMER_DPS = 0,
    TIMER_PING,
    TIMER_COUNT
} tref_t;

struct timer_spec {
    UINT_PTR id;                /* Timer ID set by timer_create(2) */
    UINT freq_ms;               /* Timer frequency */
    TIMERPROC callback;         /* Callback function called on expiry */
};

static void update_dps(HWND _1, UINT _2, UINT_PTR _3, DWORD _4);
static void ping_server(HWND _1, UINT _2, UINT_PTR _3, DWORD _4);

struct timer_spec timer_specs[] = {
    // [TIMER_DPS] = { "DPS", 0, 100 , update_dps },
    // [TIMER_PING] = { "Ping", 0, 1000, ping_server }
    { 0, 100 , update_dps },
    { 0, 1000, ping_server }
};

void
initialize_timers(void)
{
    for (int i = 0; i < TIMER_COUNT; i++) {
        timer_specs[i].id = SetTimer(NULL, 0, timer_specs[i].freq_ms, timer_specs[i].callback);
        if (timer_specs[i].id == 0) {
            print_system_error("SetTimer");
        }
    }
}

static void
update_dps(HWND _1, UINT _2, UINT_PTR _3, DWORD _4)
{
    FOPDData *data = FOPDData::getInstance();
    data->updateDPS();
}

static void
ping_server(HWND _1, UINT _2, UINT_PTR _3, DWORD _4)
{
    struct in_addr server_addr;
    FOPDData *data = FOPDData::getInstance();
    uint32_t ping_ms;
    server fo_server;

    fo_server = fo_servers[data->getServerIndex()];

    if (inet_pton(AF_INET, fo_server.address, &server_addr) != 1) {
        fprintf(stderr, "[ERROR] Fail to convert server IP to binary form (%s => %s)\n", fo_server.name, fo_server.address);
        return;
    }

    ping_ms = ping(server_addr.S_un.S_addr);
    if (ping_ms < 0) {
        fprintf(stderr, "[ERROR] Fail to ping FO server\n");
        ping_ms = 9999;
    }

    data->setPing(ping_ms);
}
