#include <iostream>
#include <cstdint>
#include <chrono>
#include <thread>

#include <stdio.h>

#include <tins/tins.h>

#include "fopd/fopd_consts.h"
#include "fopd/fopd_data.h"

#ifndef PING_ERROR
#define PING_ERROR  9999
#endif

using namespace std;

static const uint32_t get_ping_ms(string server_addr);

void ping_thread(uint32_t update_delta_ms)
{
    FOPDData *data = FOPDData::getInstance();

    while (true) {
        const uint32_t ping = get_ping_ms(FIESTA_ONLINE_SERVER_ADDRESS);
        cout << "Ping: " << static_cast<int>(ping) << "ms" << endl;
        data->setPing(ping);
        // Pause current thread
        this_thread::sleep_for(chrono::milliseconds(update_delta_ms));
    }
}

static const uint32_t get_ping_ms(string server_addr)
{
    char buffer[1024] = { 0 };
    char cmd[256] = { 0 };
    uint32_t ping = PING_ERROR;

    // Build the ping command
    snprintf(cmd, sizeof(cmd), "ping %s -n 1", server_addr.c_str());

    // Execute the command in a pipe
    FILE *p = _popen(cmd, "r");
    if (p == NULL) {
        cerr << "popen returned NULL" << endl;
        return PING_ERROR;
    }

    // Read the output
    fread(buffer, sizeof(char), 1024, p);

    // Parse output
    char *tmp = strrchr(buffer, ' ');
    if (tmp == NULL) {
        cerr << "strrchr returned NULL" << endl;
        goto CLEANUP;
    }

    // Convert the value to an int
    ping = static_cast<uint32_t>(strtol(tmp, NULL, 10));

    // strtol returns 0 if no valid value in string
    if (ping == 0) {
        ping = PING_ERROR;
    }

CLEANUP:
    if (_pclose(p) != 0) {
        ping = PING_ERROR;
    }

    return ping;
}