#ifndef __FOPD_DATA_H__
#define __FOPD_DATA_H__

#include <mutex>
#include <queue>
#include <map>
#include <vector>
#include <string.h>
#include <time.h>
#include <winsock2.h>

#include <fopd/fopd_packet.h>
#include <fopd/data_stream.h>

#define ROLLING_AVERAGE_POINT_COUNT     50


struct friend_info {
    time_t last_seen;                   /* Last time the player was seen */
    char name[FO_PLAYER_NAME_STR_LEN];  /* Player name */
    uint8_t pclass;                     /* Player class (see foclass)*/
    uint8_t level;                      /* Player level */
    char raw_map[FO_MAP_NAME_STR_LEN];  /* Player raw map name (see fopd_translation.h) */
};

class FOPDData
{
public:
    static FOPDData *getInstance(void);

    void pushToDamageQueue(struct fopacket_dmg packet_dmg);
    /**
     * @brief Update overall DPS based on the current state of the damage queue
     */
    void updateDPS(void);

    /**
     * @brief Set (or update) the "Find friends" information internally
     *
     * @param friends Friend find packet received from the server
     */
    void setFriendInfos(struct fopacket_friend_find *friends);
    void setPlayerID(struct fopacket_player_init *player);
    void setDPS(uint32_t dps);
    void setPing(uint32_t ping);
    void setTargetRemainingHealth(uint32_t target_health);
    bool setServerIndex(size_t server_index);

    std::vector<struct friend_info*> getFriendInfos(void);
    char *getPlayerName(uint16_t player_id);
    uint32_t getDPS(void);
    std::map<uint16_t, double> FOPDData::getDPSAveragePerPlayer(void);
    std::map<uint16_t, uint32_t> FOPDData::getDPSPerPlayer(void);
    std::map<uint16_t, uint32_t> FOPDData::getMaxDmgPerPlayer(void);
    uint32_t getMaxDPS(void);
    uint32_t getMaxDmg(void);
    uint32_t getPing(void);
    uint32_t getTargetRemainingHealth(void);
    double getDPSAverage(void);
    double getDPSRollingAverage(void);
    size_t getServerIndex(void);

    void operator=(const FOPDData &) = delete;
    FOPDData(FOPDData &other) = delete;

private:
    std::mutex lock;

    /* Data gathered by other sniffer and ping threads */
    uint32_t ping = 0;                      /* Current server ping latency in ms */
    uint32_t target_remaining_health = 0;   /* Current target remaining HP */
    std::queue<struct fopacket_dmg> dmg_q;  /* Queue to store damage packets to */
    std::map<char*,  struct friend_info*, cmp_str> friends; /* "Find friends" information mapped by player name */
    std::map<uint16_t, char *> player_ids;          /* Map from player ID to player name */
    std::map<uint16_t, uint32_t> dps_pp;            /* Map from player ID to current DPS */
    std::map<uint16_t, struct data_stream> avg_pp;  /* Map from player ID to average DPS */
    std::map<uint16_t, uint32_t> max_pp;            /* Map from player ID to max damage */

    uint32_t dps = 0;
    uint32_t max_dps = 0;
    uint32_t max_dmg = 0;
    struct data_stream average_dps = DATA_STREAM_INIT;
    uint32_t rolling_average_arr[ROLLING_AVERAGE_POINT_COUNT] = { 0 };
    uint32_t rolling_average_next = 0;

    /* User parameters */
    size_t server_index = 0;

    FOPDData(void);

    /**
     * @brief Attempt to set damage as the new maximum damage recorded.
     *
     * @param damage Damage that may become the max damage ever recorded
     */
    void trySetMaxDmg(uint32_t damage);

    /**
     * @brief Update the DPS average based on the current DPS, this function
     * should be called at regular intervals.
     */
    void updateDPSAverage(void);

    /**
     * @brief Update the DPS rolling average based on the current DPS.
     */
    void updateDPSRollingAverage(void);

    /**
     * @brief Update the DPS of each player based on the current state of the
     * damage queue.
     */
    void updateDPSPerPlayer(void);

    /**
     * @brief Update the average DPS of each player based on the previous DPS
     * average and the current DPS per player.
     */
    void updateDPSAveragePerPlayer(void);

};

#endif // __FOPD_DATA_H__
