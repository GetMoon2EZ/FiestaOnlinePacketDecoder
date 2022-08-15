#ifndef __FOPD_TYPES_H__
#define __FOPD_TYPES_H__

typedef enum {
    FOPD_OK,
    FOPD_ERROR
} fopd_status_t;

typedef enum {
    FOPD_UNKNOWN_PACKET,
    FOPD_DAMAGE_PACKET,
    FOPD_ENTITY_CLICK_PACKET
} fopd_packet_type_t;

#endif // __FOPD_TYPES_H__