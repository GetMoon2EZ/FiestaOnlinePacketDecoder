#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <winsock2.h>
#include <iphlpapi.h>
#include <icmpapi.h>

#include "fopd/fo_ping.h"
#include "fopd/fopd_utils.h"


#define PING_ERROR      99999
#define PING_DATA_SIZE  32

uint32_t
ping(uint32_t ipAddr)
{
    uint32_t roundTripTime = PING_ERROR;
    HANDLE hIcmpFile;
    LPVOID replyBuffer;
    DWORD replySize;
    DWORD echoRet;
    PICMP_ECHO_REPLY pEchoReply;
    char data[PING_DATA_SIZE];

    /* Data initialization */
    for (int i = 0; i < PING_DATA_SIZE - 1; i++) {
        data[i] = i + '0';
    }
    data[PING_DATA_SIZE - 1] = 0;

    hIcmpFile = IcmpCreateFile();
    if (hIcmpFile == INVALID_HANDLE_VALUE) {
        print_system_error("IcmpCreateFile");
        return PING_ERROR;
    }

    replySize = sizeof(ICMP_ECHO_REPLY) + sizeof(data);
    replyBuffer = malloc(replySize);
    if (replyBuffer == NULL) {
        fprintf(stderr, "[ERROR] Memory allocation error\n");
        IcmpCloseHandle(hIcmpFile);
        return PING_ERROR;
    }

    echoRet = IcmpSendEcho(hIcmpFile, ipAddr, data, PING_DATA_SIZE, NULL, replyBuffer, replySize, 10000);
    if (echoRet == 0) {
        print_system_error("IcmpSendEcho");
        goto exit;
    }

    pEchoReply = (PICMP_ECHO_REPLY) replyBuffer;
    roundTripTime = (uint32_t) pEchoReply->RoundTripTime;

exit:
    IcmpCloseHandle(hIcmpFile);
    free(replyBuffer);
    return roundTripTime;
}
