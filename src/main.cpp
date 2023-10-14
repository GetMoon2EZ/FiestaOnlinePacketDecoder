#include <iostream>
#include <cstdint>
#include <thread>

#define _WINSOCKAPI_    // stops windows.h including winsock.h
#include <tins/tins.h>

#ifndef FOPD_DEBUG
    #include <windows.h>
#endif

#include "fopd/packet_sniffer.h"
#include "fopd/fo_timer.h"
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
    initialize_timers();

    thread t1(sniffer_thread);
    t1.detach();

    // Run the UI in the main thread
    // Exit whenever the GUI returns (ie: window is closed)
    run_gui();

    return 0;
}
