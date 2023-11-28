#ifndef __FOPD_PLAYER_H__
#define __FOPD_PLAYER_H__

#include <stdint.h>

#include "fopd/fopd_packet.h"
#include "fopd/data_stream.h"

struct player {
    char name[FO_PLAYER_NAME_STR_LEN];  /* Player name (pseudo) */
    char map[FO_MAP_NAME_STR_LEN];      /* Current map name (network name) */
    uint16_t gid;                       /* Game ID, not an actual ID in the app */
    uint8_t level;                      /* Player in game level */
    uint32_t max_hp;                    /* Player max HP */
    uint32_t current_hp;                /* Player current HP */
    uint32_t max_sp;                    /* Player max SP (mana) */
    uint32_t current_sp;                /* Player current SP (mana) */
    uint32_t max_dmg;                   /* Max dmg (during runtime) */
    struct data_stream dps_avg;         /* Average DPS */
    struct data_stream dps_if;          /* Average in fight */
    uint32_t current_dps;               /* Current DPS (real time) */
};

struct player *player_init(void);

#endif // __FOPD_PLAYER_H__
