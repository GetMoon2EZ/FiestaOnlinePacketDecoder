#pragma once // TODO: Header guards

#include <mutex>

#define ROLLING_AVERAGE_POINT_COUNT     50

class FOPDData
{
public:
    static FOPDData *getInstance(void);

    void setDPS(uint32_t dps);
    void setPing(uint32_t ping);
    void setTargetRemainingHealth(uint32_t target_health);

    uint32_t getDPS(void);
    uint32_t getMaxDPS(void);
    uint32_t getPing(void);
    uint32_t getTargetRemainingHealth(void);
    double getDPSAverage(void);
    double getDPSRollingAverage(void);

    void operator=(const FOPDData &) = delete;
    FOPDData(FOPDData &other) = delete;

private:
    std::mutex lock;

    uint32_t dps = 0;
    uint32_t max_dps = 0;
    double average_dps = 0;
    uint32_t average_dps_n = 0;
    uint32_t rolling_average_arr[ROLLING_AVERAGE_POINT_COUNT] = { 0 };
    uint32_t rolling_average_next = 0;

    uint32_t ping = 0;
    uint32_t target_remaining_health = 0;

    FOPDData(void);
    void updateDPSAverage(void);
    void updateDPSRollingAverage(void);
};
