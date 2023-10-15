#ifndef __FOPD_DATA_H__
#define __FOPD_DATA_H__

#include <mutex>
#include <queue>
#include <map>
#include <vector>
#include <string.h>
#include <winsock2.h>

#include <fopd/fopd_packet.h>

#define ROLLING_AVERAGE_POINT_COUNT     50


struct friend_info {
    struct timeval last_seen;           /* Last time the player was seen */
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
    void updateDPS(void);
    void setFriendInfos(struct fopacket_friend_find *friends);

    void setDPS(uint32_t dps);
    void setPing(uint32_t ping);
    void setTargetRemainingHealth(uint32_t target_health);
    void trySetMaxDmg(uint32_t damage);
    bool setServerIndex(size_t server_index);

    std::vector<struct friend_info*> getFriendInfos(void);
    uint32_t getDPS(void);
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
    uint32_t ping = 0;
    uint32_t target_remaining_health = 0;
    std::queue<fopacket_dmg> dmg_q;
    std::map<char*,  struct friend_info*, cmp_str> friends;

    /* Internally calculated values */
    uint32_t dps = 0;
    uint32_t max_dps = 0;
    uint32_t max_dmg = 0;
    double average_dps = 0;
    uint32_t average_dps_n = 0;
    uint32_t rolling_average_arr[ROLLING_AVERAGE_POINT_COUNT] = { 0 };
    uint32_t rolling_average_next = 0;

    /* User parameters */
    size_t server_index = 0;

    FOPDData(void);
    void updateDPSAverage(void);
    void updateDPSRollingAverage(void);
};

#endif // __FOPD_DATA_H__
