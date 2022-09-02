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
    this->lock.lock();
    this->dps = dps;
    if (dps > this->max_dps)
        this->max_dps = dps;
    this->lock.unlock();
}

void FOPDData::setPing(uint32_t ping)
{
    this->lock.lock();
    this->ping = ping;
    this->lock.unlock();
}

uint32_t FOPDData::getDPS(void)
{
    this->lock.lock();
    uint32_t _dps = this->dps;
    this->lock.unlock();
    return _dps;
}

uint32_t FOPDData::getMaxDPS(void)
{
    this->lock.lock();
    uint32_t _max_dps = this->max_dps;
    this->lock.unlock();
    return _max_dps;
}

uint32_t FOPDData::getPing(void)
{
    this->lock.lock();
    uint32_t _ping = this->ping;
    this->lock.unlock();
    return _ping;
}