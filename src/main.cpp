#include <iostream>
#include <cstdint>
#include <thread>

#include <tins/tins.h>

#ifndef FOPD_DEBUG
    #include <windows.h>
#endif

#include "fopd/dps_meter.h"
#include "fopd/packet_sniffer.h"
#include "fopd/ping.h"
#include "fopd/gui.h"


using namespace std;

#ifdef FOPD_DEBUG
int main()
#else
int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nShowCmd)
#endif
{
    fopd_damage_queue dmg_q;

    sniffer_thread(&dmg_q);

    return 0;
}
