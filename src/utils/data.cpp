#include "fopd/fopd_data.h"

#include <numeric>
#include <algorithm>
#include <queue>
#include <map>
#include <vector>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <winsock2.h>

#include "fopd/fopd_consts.h"
#include "fopd/fopd_packet.h"
#include "fopd/fopd_utils.h"


static FOPDData *instance = NULL;

static uint32_t get_dmg_queue_max(std::queue<fopacket_dmg> q);
static uint32_t get_dmg_queue_sum(std::queue<fopacket_dmg> q);

/****************/
/* Constructors */
/****************/

FOPDData::FOPDData(void) {}

/******************/
/* Public Methods */
/******************/

FOPDData *FOPDData::getInstance(void)
{
    if (instance == NULL) {
        instance = new FOPDData();
    }
    return instance;
}

/***********/
/* Setters */
/***********/

void
FOPDData::pushToDamageQueue(struct fopacket_dmg dmg)
{
    std::lock_guard<std::mutex> lk(this->lock);
    this->dmg_q.push(dmg);
}

void
FOPDData::updateDPS(void)
{
    std::lock_guard<std::mutex> lk(this->lock);
    struct timeval now, diff;
    fopacket_dmg curr_dmg;

    if (gettimeofday(&now, NULL) != 0) {
        fprintf(stderr, "[ERROR] gettimeofday returned an error: %s\n", strerror(errno));
        return;
    }

    while (!this->dmg_q.empty()) {
        curr_dmg = this->dmg_q.front();
        if (timeval_subtract(now, curr_dmg.timestamp, &diff) != 0) {
            /* Packet from the future ? */
            fprintf(stdout, "[DEBUG] packet from the future ?\n");
            continue;
        }

        if (diff.tv_sec == 0 && diff.tv_usec < 1000000) {
            /* Stop at the first packet within the last second */
            break;
        }

        /* Remove packets which are too old */
        this->dmg_q.pop();
    }

    /* Sum damage of from the last second */
    this->trySetMaxDmg(get_dmg_queue_max(this->dmg_q));
    this->setDPS(get_dmg_queue_sum(this->dmg_q));
}

void
FOPDData::setFriendInfos(struct fopacket_friend_find *pkt_friends)
{
    std::lock_guard<std::mutex> lk(this->lock);
    time_t now;
    struct friend_info *finfo;

    now = time(NULL);

    for (uint8_t i = 0; i < pkt_friends->num_entry; i++) {
        finfo = (struct friend_info*) calloc(1, sizeof(*finfo));
        if (finfo == NULL) {
            fprintf(stderr, "[ERROR] Memory allocation error\n");
            return;
        }
        finfo->last_seen = now;
        finfo->level = pkt_friends->players[i].level;
        finfo->pclass = pkt_friends->players[i].pclass;
        strncpy(finfo->raw_map, pkt_friends->players[i].raw_map, FO_MAP_NAME_MAX_LEN);
        finfo->raw_map[FO_MAP_NAME_MAX_LEN] = '\0';
        strncpy(finfo->name, pkt_friends->players[i].name, FO_PLAYER_NAME_MAX_LEN);
        finfo->name[FO_PLAYER_NAME_MAX_LEN] = '\0';

        /* Insert in the map */
        auto old_info = this->friends.find(finfo->name);
        if (old_info != this->friends.end()) {
            auto x = std::move(old_info->second);
            this->friends.erase(old_info);
            free(x);
        }
        this->friends[finfo->name] = finfo;
    }
}

// TODO Periodically remove friend informations which are too old (and free memory)
// void FOPDData::cleanupFriend(void);

void FOPDData::setDPS(uint32_t dps)
{
    // std::lock_guard<std::mutex> lk(this->lock);
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

bool FOPDData::setServerIndex(size_t server_index)
{
    if (server_index >= SERVER_COUNT) {
        return false;
    }

    std::lock_guard<std::mutex> lk(this->lock);
    this->server_index = server_index;
    return true;
}

void FOPDData::trySetMaxDmg(uint32_t damage)
{
    // std::lock_guard<std::mutex> lk(this->lock);
    this->max_dmg = (std::max)(this->max_dmg, damage);
}

/***********/
/* Getters */
/***********/

bool
compareByTimestamp(const struct friend_info *a, const struct friend_info *b)
{
    return a->last_seen > b->last_seen;
}

std::vector<struct friend_info*>
FOPDData::getFriendInfos(void)
{
    std::vector<struct friend_info*> v;

    std::lock_guard<std::mutex> lk(this->lock);
    for (auto const& x : this->friends) {
        v.push_back(x.second);
    }

    std::sort(v.begin(), v.end(), compareByTimestamp);
    return v;
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


double FOPDData::getDPSAverage(void)
{
    std::lock_guard<std::mutex> lk(this->lock);
    return this->average_dps;
}

double FOPDData::getDPSRollingAverage(void)
{
    std::lock_guard<std::mutex> lk(this->lock);
    double average = 0;
    uint32_t n = 0;
    for (uint32_t value: this->rolling_average_arr) {
        average += ((double) value - average) / (double) ++n;
    }
    return average;
}

size_t FOPDData::getServerIndex(void)
{
    // std::lock_guard<std::mutex> lk(this->lock);
    return this->server_index;
}

/*******************/
/* Private Methods */
/*******************/

void FOPDData::updateDPSAverage(void)
{
    // a = a + ( v - a ) / (n + 1)
    this->average_dps = this->average_dps + ((double) this->dps - this->average_dps) / (double) (++this->average_dps_n);
}

void FOPDData::updateDPSRollingAverage(void)
{
    this->rolling_average_arr[this->rolling_average_next] = this->dps;
    this->rolling_average_next = (this->rolling_average_next + 1) % ROLLING_AVERAGE_POINT_COUNT;
}

/********************/
/* HELPER FUNCTIONS */
/********************/

static uint32_t
get_dmg_queue_max(std::queue<fopacket_dmg> q)
{
    uint32_t max_dmg = 0;
    while (!q.empty()) {
        for (uint8_t i = 0; i < q.front().hit_cnt; i++) {
            if (q.front().dinfo[i].inflicted_dmg > FO_MAX_POSSIBLE_DAMAGE) {
                continue;
            }
            max_dmg = (std::max)(max_dmg, q.front().dinfo[i].inflicted_dmg);
        }
        q.pop();
    }
    return max_dmg;
}

static uint32_t
get_dmg_queue_sum(std::queue<fopacket_dmg> q)
{
    uint32_t sum = 0;
    while (!q.empty()) {
        for (uint8_t i = 0; i < q.front().hit_cnt; i++) {
            if (q.front().dinfo[i].inflicted_dmg > FO_MAX_POSSIBLE_DAMAGE) {
                continue;
            }

            sum += q.front().dinfo[i].inflicted_dmg;
        }
        q.pop();
    }
    return sum;
}
