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
#include "fopd/data_stream.h"


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
    this->updateDPSPerPlayer();
    this->updateDPSAveragePerPlayer();
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

void
FOPDData::setPlayerID(struct fopacket_player_init *pkt_player)
{
    char *player_name;
    auto old_player = this->player_ids.find(pkt_player->player_id);
    uint16_t old_id;
    bool had_old_id;

    /* Remove the player which used to have this ID */
    if (old_player != this->player_ids.end()) {
        if (strncmp(old_player->second, pkt_player->name, FO_PLAYER_NAME_MAX_LEN) == 0) {
            return;
        }
        /* Free old player's name memory */
        auto old_name = std::move(old_player->second);
        this->player_ids.erase(old_player);
        free(old_name);
    }

    /* Remove the old ID for this player */
    /* This requires a reverse search but should not be done very often */
    had_old_id = false;
    for (auto it = this->player_ids.begin(); it != this->player_ids.end(); ++it) {
        if (strncmp(it->second, pkt_player->name, FO_PLAYER_NAME_MAX_LEN) == 0) {
            old_id = it->first;
            had_old_id = true;
        }
    }

    if (had_old_id) {
        auto old_id_p = this->player_ids.find(old_id);
        auto x = std::move(old_id_p->second);
        this->player_ids.erase(old_id_p);
        free(x);
    }

    /* Insert the player in the map */
    player_name = (char *) calloc(FO_PLAYER_NAME_STR_LEN, sizeof(*player_name));
    if (player_name == NULL) {
        fprintf(stderr, "[ERROR] Memory allocation error\n");
        return;
    }

    strncpy(player_name, pkt_player->name, FO_PLAYER_NAME_MAX_LEN);
    player_name[FO_PLAYER_NAME_MAX_LEN] = '\0';
    this->player_ids[pkt_player->player_id] = player_name;
    fprintf(stderr, "Saved player: %s (%u)\n", player_name, pkt_player->player_id);
}

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

char *
FOPDData::getPlayerName(uint16_t player_id)
{
    auto player = this->player_ids.find(player_id);

    if (player == this->player_ids.end()) {
        return "Unknown";
    }

    return player->second;
}

uint32_t FOPDData::getDPS(void)
{
    std::lock_guard<std::mutex> lk(this->lock);
    return this->dps;
}

std::map<uint16_t, double>
FOPDData::getDPSAveragePerPlayer(void)
{
    std::lock_guard<std::mutex> lk(this->lock);
    std::map<uint16_t, double> ret;

    for (auto const &x: this->avg_pp) {
        ret[x.first] = x.second.avg;
    }
    return ret;
}

std::map<uint16_t, uint32_t>
FOPDData::getDPSPerPlayer(void)
{
    std::lock_guard<std::mutex> lk(this->lock);
    return this->dps_pp;
}

std::map<uint16_t, uint32_t>
FOPDData::getMaxDmgPerPlayer(void)
{
    std::lock_guard<std::mutex> lk(this->lock);
    return this->max_pp;
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
    return this->average_dps.avg;
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
    data_stream_push(&this->average_dps, this->dps);
}

void FOPDData::updateDPSRollingAverage(void)
{
    this->rolling_average_arr[this->rolling_average_next] = this->dps;
    this->rolling_average_next = (this->rolling_average_next + 1) % ROLLING_AVERAGE_POINT_COUNT;
}

void
FOPDData::updateDPSPerPlayer(void)
{
    std::queue<struct fopacket_dmg> copy_q = this->dmg_q;

    /* Reset previous DPS, it will be recalculated now */
    for (auto const &x: this->dps_pp) {
        this->dps_pp[x.first] = 0;
    }

    while (!copy_q.empty()) {
        struct fopacket_dmg pkt_dmg = copy_q.front();
        uint16_t origin_id = pkt_dmg.origin_id;
        uint32_t max_dmg = 0;

        /* Initialize at 0 */
        auto x_dps = this->dps_pp.find(origin_id);
        if (x_dps == this->dps_pp.end()) {
            this->dps_pp[origin_id] = 0;
        }

        for (uint8_t i = 0; i < pkt_dmg.hit_cnt; i++) {
            this->dps_pp[origin_id] += pkt_dmg.dinfo[i].inflicted_dmg;

            if (pkt_dmg.dinfo[i].inflicted_dmg > max_dmg) {
                max_dmg = pkt_dmg.dinfo[i].inflicted_dmg;
            }
        }

        /* Update the max damage for this player */
        auto x_max = this->max_pp.find(origin_id);
        if (x_max == this->max_pp.end()) {
            this->max_pp[origin_id] = max_dmg;
        } else if (this->max_pp[origin_id] < max_dmg) {
            this->max_pp[origin_id] = max_dmg;
        }

        copy_q.pop();
    }
}

void
FOPDData::updateDPSAveragePerPlayer(void)
{
    for (const auto &player_dps: this->dps_pp) {
        uint32_t dps = 0;
        uint16_t player_id = player_dps.first;

        /* Find current DPS of the player */
        // auto x_dps = this->dps_pp.find(player_id);
        // if (x_dps != this->dps_pp.end()) {
        //     dps = this->dps_pp[player_id];
        // }

        /* Find the recorded average DPS for the player */
        auto x_avg = this->avg_pp.find(player_id);
        if (x_avg == this->avg_pp.end()) {
            this->avg_pp[player_id] = DATA_STREAM_INIT;
        }
        data_stream_push(&this->avg_pp[player_id], player_dps.second);
    }
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
