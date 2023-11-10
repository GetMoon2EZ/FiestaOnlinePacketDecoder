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

// #include <fstream>
// #include <sstream>
// #include <string>
// #include <fopd/packet_sniffer.h>
// #include <stdint.h>
// #include <map>

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
    /** BEGIN TEST **/
    // std::ifstream infile("./test/test_set.txt");
    // std::string line;
    // std::string del = ":";
    // std::map<char, uint8_t> hex {
    //     { '0', 0 },
    //     { '1', 1 },
    //     { '2', 2 },
    //     { '3', 3 },
    //     { '4', 4 },
    //     { '5', 5 },
    //     { '6', 6 },
    //     { '7', 7 },
    //     { '8', 8 },
    //     { '9', 9 },
    //     { 'A', 10 },
    //     { 'B', 11 },
    //     { 'C', 12 },
    //     { 'D', 13 },
    //     { 'E', 14 },
    //     { 'F', 15 }
    // };

    // while (std::getline(infile, line)) {
    //     std::string raw_data = line.substr(line.find(del) + 1, line.length());
    //     uint32_t data_len = (uint32_t) raw_data.length() / 3;
    //     uint8_t *data = (uint8_t*) malloc(data_len);
    //     printf("%d\n", data_len);
    //     for (uint32_t i = 0; i < data_len; i++) {
    //         data[i] = (hex[raw_data[i * 3 + 1]] << 4) + hex[raw_data[i * 3 + 2]];
    //     }
    //     test_packet_split(data, data_len);
    //     free(data);
    // }
    // return 0;
    /** END TEST **/

    initialize_timers();

    thread t1(sniffer_thread);
    t1.detach();

    // Run the UI in the main thread
    // Exit whenever the GUI returns (ie: window is closed)
    run_gui();

    return 0;
}
