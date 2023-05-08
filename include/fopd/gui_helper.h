#pragma once

#include <stdint.h>

#define MEMORY_DISPLAY_BUFFER_SIZE  32

struct MemoryInfo {
    /* Virtual Memory */
    uint64_t totalVirtualMem;
    uint64_t virtualMemUsed;
    uint64_t virtualMemUsedByProcess;
    /* Physical Memory */
    uint64_t totalPhysMem;
    uint64_t physMemUsed;
    uint64_t physMemUsedByProcess;
};

struct MemoryInfo get_memory_info(void);
void memory_display(uint64_t bytes, char *buf);
