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
    if (dps > this->max_dps)
        this->max_dps = dps;
}

void FOPDData::setPing(uint32_t ping)
{
    std::lock_guard<std::mutex> lk(this->lock);
    this->ping = ping;
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

uint32_t FOPDData::getPing(void)
{
    std::lock_guard<std::mutex> lk(this->lock);
    return this->ping;
}