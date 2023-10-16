#ifndef __FOPD_PACKETS_H__
#define __FOPD_PACKETS_H__

#include <stdint.h>
#include <winsock2.h>

#include <fopd/fopd_utils.h>


#define FO_PACKET_MAX_DATA_LEN  4096
#define FO_PACKET_MIN_LEN       3       /* len on 1 byte, type on 2 bytes */
#define FO_SPELL_MAX_TARGET_HIT 20      /* No spell can hit more than 20 targets ? */
#define FO_MAX_POSSIBLE_DAMAGE  1000000000ULL // 1 Billion
#define FO_PLAYER_NAME_MAX_LEN  16      /* Maximum length for a player name */
#define FO_PLAYER_NAME_STR_LEN  (FO_PLAYER_NAME_MAX_LEN + 1) /* Maximum length for a player name */
#define FO_MAP_NAME_MAX_LEN     12      /* Maximum length for a map name */
#define FO_MAP_NAME_STR_LEN     (FO_MAP_NAME_MAX_LEN + 1)
#define FO_FRIEND_MAX_PLAYERS   40      /* Maximum number of player info per packet */


enum errors {
    INPUT_ERROR = -2,
    INTERNAL_ERROR = -1,
    SUCCESS = 0,
};

enum fopacket_types {
    FOPACKET_DMG_AA =       0x2448,
    FOPACKET_DMG_SPELL =    0x2452,
    FOPACKET_ENTITY_INFO =  0x2402,
    FOPACKET_FRIEND_LOGIN = 0x743D,
    FOPACKET_FRIEND_LOGOUT = 0x540A,
    FOPACKET_FRIEND_FIND =  0x5420,
};

/* Generic packet */
PACK(struct fopacket {
    uint16_t len;
    uint16_t type;
    uint8_t data[FO_PACKET_MAX_DATA_LEN];
});

PACK(struct dmg_info {
    uint16_t target_id;     /* ID of the target receiving the hit */
    uint8_t _unknown_1[2];  /* Not known, set for packing */
    uint32_t inflicted_dmg; /* Inflicted damage */
    uint32_t remaining_hp;  /* Target remaining HP after the hit*/
    uint8_t _unknown_2[2];  /* Not known, set for packing */
});

/* Damage inflicted by auto attacks */
PACK(struct fopacket_dmg_aa {
    uint16_t len;           /* Packet length */
    uint16_t type;          /* Packet header */
    uint16_t origin_id;     /* ID of the player/mob dealing the hit */
    /* Bellow looks the same as struct dmg_info */
    // uint16_t target_id;     /* ID of the target receiving the hit */
    // uint8_t _unknown_1[2];  /* Not known, set for packing */
    // uint32_t inflicted_dmg; /* Inflicted damage */
    // uint32_t remaining_hp;  /* Target remaining HP after the hit*/
    // uint8_t _unknown_2[4];  /* Not known, set for packing */
    struct dmg_info dinfo; /* Damages dealt by the auto attack*/
    uint8_t _unknown_2[2];
});

/* Damage inflicted by spells */
PACK(struct fopacket_dmg_spell {
    uint16_t len;           /* Packet length */
    uint16_t type;          /* Packet header */
    uint8_t _unknown_1[2];  /* Not known, set for packing */
    uint16_t origin_id;     /* ID of the player/mob dealing the hit */
    uint8_t _unknown_2[5];  /* Not known, set for packing */
    struct dmg_info dinfo[FO_SPELL_MAX_TARGET_HIT];  /* Damages dealt by the spell on one or multiple targets */
});

struct fopacket_dmg {
    struct timeval timestamp;
    uint16_t len;
    uint16_t type;
    uint16_t origin_id;
    uint8_t hit_cnt; /* Number of targets hit */
    struct dmg_info dinfo[FO_SPELL_MAX_TARGET_HIT];
};

PACK(struct fopacket_entity_info {
    uint16_t len;           /* Packet length */
    uint16_t type;          /* Packet header */
    uint16_t id;            /* ID of the entity */
    uint32_t current_hp;    /* Current HP */
    uint32_t max_hp;        /* Maximum HP */
    uint32_t current_sp;    /* Current SP*/
    uint32_t max_sp;        /* Maximum SP */
    uint8_t _unknown_1[8];  /* Not known, set for packing */
    uint8_t level;          /* Entity level [1-150]*/
});

PACK(struct player_info {
    uint8_t _unknown_1[4];              /* Not known, set for packing */
    char name[FO_PLAYER_NAME_MAX_LEN];  /* Player name */
    uint8_t _unknown_2[4];              /* Not known, set for packing */
    uint8_t pclass;                     /* Player class (see foclass)*/
    uint8_t level;                      /* Player level */
    uint8_t _unknown_3[2];              /* Not known, set for packing */
    char raw_map[FO_MAP_NAME_MAX_LEN];  /* Player raw map name (see fopd_translation.h) */
    uint8_t _unknown_4[32];             /* Not known, set for packing */
});

PACK(struct fopacket_friend_find {
    uint16_t len;           /* Packet length */
    uint16_t type;          /* Packet header */
    uint16_t subtype;       /* Packet sub-header (C0 36 for player info ?) */
    uint8_t num_entry;      /* Number of entries in the following array */
    uint8_t _unknown_1[1];  /* Not known, set for packing */
    struct player_info players[FO_FRIEND_MAX_PLAYERS];
});

void print_packet(const struct fopacket *packet);
int get_payload_len(const uint8_t *buf, uint32_t buf_size, uint32_t *payload_len);
int parse_packet(const uint8_t *buf, uint32_t buf_size, struct fopacket *packet);
int parse_packet_damage(const struct fopacket *packet, struct fopacket_dmg *out);
int parse_packet_entity_info(const struct fopacket *packet, struct fopacket_entity_info *out);
int parse_packet_friend_find(const struct fopacket *packet, struct fopacket_friend_find *out);
int handle_damage(struct fopacket *packet);
int handle_entity_info(struct fopacket *packet);
int handle_friend_find(struct fopacket *packet);

#endif // __FOPD_PACKETS_H__
