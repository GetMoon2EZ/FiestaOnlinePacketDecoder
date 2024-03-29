#pragma once

#include <cstdint>
#include <string>

/*****************/
/* Game Servers */
/*****************/

/* French servers */
#define FIESTA_ONLINE_FR_CYPIAN_ADDRESS     "35.189.254.31"
#define FIESTA_ONLINE_FR_HEDNIS_ADDRESS     "35.189.254.31"
#define FIESTA_ONLINE_FR_ALBIREO_ADDRESS    "35.189.254.31"
/* Spanish servers */
#define FIESTA_ONLINE_ES_ENID_ADDRESS       "35.233.4.109"
/* German servers */
#define FIESTA_ONLINE_DE_KHAZUL_ADDRESS     "34.78.7.253"
#define FIESTA_ONLINE_DE_MALEPHAR_ADDRESS   "46.253.147.130"
#define FIESTA_ONLINE_DE_IYZEL_ADDRESS      "35.233.4.109"
/* NA server */
#define FIESTA_ONLINE_NA_ENID_ADDRESS       "35.185.11.255"
#define FIESTA_ONLINE_NA_PAGEL_ADDRESS      "35.185.11.255"
#define FIESTA_ONLINE_NA_ISYA_ADDRESS       "35.231.44.7"
#define FIESTA_ONLINE_NA_JENIRA_ADDRESS     "35.231.44.7"
#define FIESTA_ONLINE_NA_CYRAL_ADDRESS      "35.185.11.255"

enum {
    /* FR */
    SERVER_CYPIAN = 0,
    SERVER_HEDNIS,
    SERVER_ALBIREO,
    /* ES */
    SERVER_ENID_ES,
    /* DE */
    SERVER_KHAZUL,
    SERVER_MALEFAR,
    SERVER_IYZEL,
    /* NA */
    SERVER_ENID,
    SERVER_PAGEL,
    SERVER_ISYA,
    SERVER_JENIRA,
    SERVER_CYRAL,
    /* Total */
    SERVER_COUNT,
};

struct server {
    const char *name;
    const char *address;
};

const struct server fo_servers[SERVER_COUNT] = {
    /* FR */
    { "FR - Cypian", FIESTA_ONLINE_FR_CYPIAN_ADDRESS },
    { "FR - Hednis", FIESTA_ONLINE_FR_HEDNIS_ADDRESS },
    { "FR - Albireo", FIESTA_ONLINE_FR_ALBIREO_ADDRESS },
    /* ES */
    { "ES - Enid", FIESTA_ONLINE_ES_ENID_ADDRESS },
    /* DE */
    { "DE - Khazul", FIESTA_ONLINE_DE_KHAZUL_ADDRESS },
    { "DE - Malephar", FIESTA_ONLINE_DE_MALEPHAR_ADDRESS },
    { "DE - Iyzel", FIESTA_ONLINE_DE_IYZEL_ADDRESS },
    /* NA */
    { "NA - Enid", FIESTA_ONLINE_NA_ENID_ADDRESS },
    { "NA - Pagel", FIESTA_ONLINE_NA_PAGEL_ADDRESS },
    { "NA - Isya", FIESTA_ONLINE_NA_ISYA_ADDRESS },
    { "NA - Jenira", FIESTA_ONLINE_NA_JENIRA_ADDRESS },
    { "NA - Cyral", FIESTA_ONLINE_NA_CYRAL_ADDRESS },
};


/* Field offsets in raw tcp data */
#define FOPD_PACKET_PAYLOAD_LEN_OFFSET  0
#define FOPD_PACKET_HEADER_OFFSET       1
#define FOPD_PACKET_PAYLOAD_OFFSET      3

/* Field lengths */
#define FOPD_PACKET_HEADER_LEN          2
#define FOPD_PACKET_PAYLOAD_MAX_LEN     255



/* Spell damage packets (single target) */
enum {
    FOPD_SPELL_DAMAGE_PACKET_HEADER_B0 =    0x52,
    FOPD_SPELL_DAMAGE_PACKET_HEADER_B1 =    0x24
};

const uint8_t FOPD_SPELL_DAMAGE_PACKET_HEADER[FOPD_PACKET_HEADER_LEN] = {
    FOPD_SPELL_DAMAGE_PACKET_HEADER_B0,
    FOPD_SPELL_DAMAGE_PACKET_HEADER_B1
};

/* Auto Attack damage packets */
enum {
    FOPD_AA_DAMAGE_PACKET_HEADER_B0 =       0x48,
    FOPD_AA_DAMAGE_PACKET_HEADER_B1 =       0x24
};

const uint8_t FOPD_AA_DAMAGE_PACKET_HEADER[FOPD_PACKET_HEADER_LEN]    = {
    FOPD_AA_DAMAGE_PACKET_HEADER_B0,
    FOPD_AA_DAMAGE_PACKET_HEADER_B1
};

/* TODO AOE damage packets */

/* On entity click information packets */
enum {
    FOPD_ENTITY_CLICK_PACKET_HEADER_B0  =   0x02,
    FOPD_ENTITY_CLICK_PACKET_HEADER_B1  =   0x24
};

const uint8_t FOPD_ENTITY_CLICK_PACKET_HEADER[FOPD_PACKET_HEADER_LEN] = {
    FOPD_ENTITY_CLICK_PACKET_HEADER_B0,
    FOPD_ENTITY_CLICK_PACKET_HEADER_B1
};
