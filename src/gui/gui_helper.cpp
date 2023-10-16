#include "fopd/gui_helper.h"

#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include <windows.h>
#include <psapi.h>


#define BYTE        1ULL
#define KILOBYTE    (BYTE * 1000)
#define MEGABYTE    (KILOBYTE * 1000)
#define GIGABYTE    (MEGABYTE * 1000)
#define TERABYTE    (GIGABYTE * 1000)

#define SECOND      1ULL
#define MINUTE_S    (SECOND * 60)
#define HOUR_S      (MINUTE_S * 60)
#define DAY_S       (HOUR_S * 24)


struct MemoryInfo get_memory_info(void)
{
    struct MemoryInfo info = { 0 };
    MEMORYSTATUSEX memInfo;
    PROCESS_MEMORY_COUNTERS_EX pmc;
    DWORDLONG totalVirtualMem, virtualMemUsed, totalPhysMem, physMemUsed;
    SIZE_T virtualMemUsedByProcess, physMemUsedByProcess;

    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    totalVirtualMem = memInfo.ullTotalPageFile;
    virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
    totalPhysMem = memInfo.ullTotalPhys;
    physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;

    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

    virtualMemUsedByProcess = pmc.PrivateUsage;
    physMemUsedByProcess = pmc.WorkingSetSize;

    info.totalVirtualMem = (uint64_t) totalVirtualMem;
    info.virtualMemUsed = (uint64_t) virtualMemUsed;
    info.virtualMemUsedByProcess = (uint64_t) virtualMemUsedByProcess;
    info.totalPhysMem = (uint64_t) totalPhysMem;
    info.physMemUsed = (uint64_t) physMemUsed;
    info.physMemUsedByProcess = (uint64_t) physMemUsedByProcess;

    return info;
}

void memory_display(uint64_t bytes, char buf[MEMORY_DISPLAY_BUFFER_SIZE])
{
    double unit;
    char unit_str[6] = { 0 };

    if (bytes > TERABYTE) {
        strncpy(unit_str, "TB", sizeof(unit_str));
        unit = (double) TERABYTE;
    } else if (bytes > GIGABYTE) {
        strncpy(unit_str, "GB", sizeof(unit_str));
        unit = (double) GIGABYTE;
    } else if (bytes > MEGABYTE) {
        strncpy(unit_str, "MB", sizeof(unit_str));
        unit = (double) MEGABYTE;
    } else if (bytes > KILOBYTE) {
        strncpy(unit_str, "KB", sizeof(unit_str));
        unit = (double) KILOBYTE;
    } else {
        strncpy(unit_str, "Bytes", sizeof(unit_str));
        unit = (double) BYTE;
    }

    snprintf(buf, MEMORY_DISPLAY_BUFFER_SIZE, "%.2f%s", bytes / unit, unit_str);
}

void
time_since_str(time_t time_since, char buf[TIME_SINCE_BUFFER_SIZE])
{
    time_t diff;

    diff = time(NULL) - time_since;

    if (diff < 0) {
        diff *= -1;
    }

    if (diff < MINUTE_S) {
        snprintf(buf, TIME_SINCE_BUFFER_SIZE, "%llds", diff);
    } else if (diff < HOUR_S) {
        snprintf(buf, TIME_SINCE_BUFFER_SIZE, "%lldm%llds", diff / MINUTE_S, diff % MINUTE_S);
    } else if (diff < DAY_S) {
        snprintf(buf, TIME_SINCE_BUFFER_SIZE, "%lldh%lldm", diff / HOUR_S, (diff % HOUR_S) / MINUTE_S);
    } else {
        time_t days = diff / DAY_S;
        snprintf(buf, TIME_SINCE_BUFFER_SIZE, "%lld day%s", days, days > 1 ? "s" : "");
    }
}
