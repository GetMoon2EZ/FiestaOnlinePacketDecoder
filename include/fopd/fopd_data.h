#pragma once // TODO: Header guards

#include <mutex>

class FOPDData
{
public:
    static FOPDData *getInstance(void);

    void setDPS(uint32_t dps);
    void setPing(uint32_t ping);

    uint32_t getDPS(void);
    uint32_t getMaxDPS(void);
    uint32_t getPing(void);

    void operator=(const FOPDData &) = delete;
    FOPDData(FOPDData &other) = delete;

private:
    std::mutex lock;

    uint32_t dps = 0;
    uint32_t max_dps = 0;
    uint32_t ping = 0;
    FOPDData(void);
};
