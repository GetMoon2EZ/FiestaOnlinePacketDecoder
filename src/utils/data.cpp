#include "fopd/fopd_data.h"

static FOPDData *instance = NULL;

FOPDData::FOPDData(void) {}

FOPDData *FOPDData::getInstance(void)
{
    if (instance == NULL) {
        instance = new FOPDData();
    }
    return instance;
}

void FOPDData::setDPS(uint32_t dps)
{
    std::lock_guard<std::mutex> lk(this->lock);
    this->dps = dps;
    if (dps > this->max_dps) {
        this->max_dps = dps;
    }

    this->updateDPSAverage();
    this->updateDPSRollingAverage();
}

void FOPDData::setPing(uint32_t ping)
{
    std::lock_guard<std::mutex> lk(this->lock);
    this->ping = ping;
}

void FOPDData::setTargetRemainingHealth(uint32_t target_health)
{
    std::lock_guard<std::mutex> lk(this->lock);
    this->target_remaining_health = target_health;
}

void FOPDData::trySetMaxDmg(uint32_t damage)
{
    this->max_dmg = std::max(this->max_dmg, damage);
}

uint32_t FOPDData::getDPS(void)
{
    std::lock_guard<std::mutex> lk(this->lock);
    return this->dps;
}

uint32_t FOPDData::getMaxDPS(void)
{
    std::lock_guard<std::mutex> lk(this->lock);
    return this->max_dps;
}

uint32_t FOPDData::getMaxDmg(void)
{
    std::lock_guard<std::mutex> lk(this->lock);
    return this->max_dmg;
}

uint32_t FOPDData::getPing(void)
{
    std::lock_guard<std::mutex> lk(this->lock);
    return this->ping;
}

uint32_t FOPDData::getTargetRemainingHealth(void)
{
    std::lock_guard<std::mutex> lk(this->lock);
    return this->target_remaining_health;
}

void FOPDData::updateDPSAverage(void)
{
    // a = a + ( v - a ) / (n + 1)
    this->average_dps = this->average_dps + ((double) this->dps - this->average_dps) / (double) (++this->average_dps_n);
}

double FOPDData::getDPSAverage(void)
{
    std::lock_guard<std::mutex> lk(this->lock);
    return this->average_dps;
}

double FOPDData::getDPSRollingAverage(void)
{
    double average = 0;
    uint32_t n = 0;
    for (uint32_t value: this->rolling_average_arr) {
        average += ((double) value - average) / (double) ++n;
    }
    return average;
}

void FOPDData::updateDPSRollingAverage(void)
{
    this->rolling_average_arr[this->rolling_average_next] = this->dps;
    this->rolling_average_next = (this->rolling_average_next + 1) % ROLLING_AVERAGE_POINT_COUNT;
}
