#include "fopd/gui_helper.h"

#include <stdio.h>
#include <stdint.h>

#include "windows.h"
#include "psapi.h"


#define BYTE        1ULL
#define KILOBYTE    (BYTE * 1000)
#define MEGABYTE    (KILOBYTE * 1000)
#define GIGABYTE    (MEGABYTE * 1000)
#define TERABYTE    (GIGABYTE * 1000)

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

void memory_display(uint64_t bytes, char *buf)
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
