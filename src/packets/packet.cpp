#include "fopd/fopd_packet.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <winsock2.h>

#include "fopd/fopd_data.h"
#include "fopd/fopd_utils.h"


void
print_packet(const struct fopacket *packet)
{
    printf("[DEBUG] %u - %04X - ", packet->len, packet->type);
    if (packet->len < sizeof(packet->type)) {
        printf("\n");
        return;
    }

    for (int i = 0; i < packet->len - sizeof(packet->type); i++) {
        printf("%02X ", packet->data[i]);
    }
    printf("\n");

}

int
get_payload_len(const uint8_t *buf, uint32_t buf_size, uint32_t *payload_len)
{
    if (buf == NULL || payload_len == NULL) {
        return -1;
    }

    *payload_len = 0;
    if (buf_size == 0) {
        return 1;
    }

    if (buf_size < 3) {
        return -1;
    }

    if (buf[0] == 0) {
        (*payload_len) = buf[2] << 8;
        (*payload_len) += buf[1];
        (*payload_len) += 3; /* Account for the 3 bytes indicating the length */
    } else {
        (*payload_len) = buf[0];
        (*payload_len) += 1; /* Account for the byte indicating the length */
    }
    return 0;
}

int
parse_packet(const uint8_t *buf, uint32_t buf_size, struct fopacket *packet)
{
    uint8_t tmp[2000] = { 0 };
    uint32_t tmp_size = buf_size;

    if (buf_size < FO_PACKET_MIN_LEN || buf_size > sizeof(*packet)) {
        printf("buf_size = %u\n", buf_size);
        fprintf(stderr, "[ERROR] Invalid input\n");
        return -1;
    }

    memset(packet, 0, sizeof(*packet));
    if (buf[0] != 0) {
        /* Short length detected: shift to match long packet form */
        memcpy(tmp + 1, buf, buf_size);
        tmp[0] = tmp[1];
        tmp[1] = 0;
        tmp_size++;
    } else {
        /* Ignore the first 0x00 byte and shift the buffer */
        memcpy(tmp, buf + 1, buf_size);
        tmp_size--;
    }
    memcpy(packet, tmp, tmp_size);
    /* Flip endianness if needed */
    packet->len = letoss(packet->len);
    packet->type = letoss(packet->type);
    return 0;
}

int
parse_packet_damage(const struct fopacket *packet, struct fopacket_dmg *out)
{
    struct fopacket_dmg_aa aa;
    struct fopacket_dmg_spell spell;

    memset(out, 0, sizeof(*out));

    if (packet->type == FOPACKET_DMG_AA) {
        if (packet->len != sizeof(aa) - sizeof(aa.len)) {
            return -1;
        }

        memcpy(&aa, packet, sizeof(aa));

        out->len = aa.len;
        out->type = aa.type;
        out->origin_id = aa.origin_id;
        out->hit_cnt = 1;
        out->dinfo[0] = aa.dinfo;
    } else if (packet->type == FOPACKET_DMG_SPELL) {
        /*
            Length check: the packet can contain from one to FO_SPELL_MAX_TARGET_HIT
            struct dmg_info. Also assert that the length includes at least one
            dmg_info.
        */
        uint32_t noinfo_len = sizeof(spell) - sizeof(spell.len) - sizeof(struct dmg_info) * FO_SPELL_MAX_TARGET_HIT;
        uint8_t hit_cnt = (uint8_t) (packet->len - noinfo_len) / sizeof(struct dmg_info);

        if (
            packet->len > (sizeof(spell) - sizeof(spell.len)) ||
            (packet->len % sizeof(struct dmg_info)) != noinfo_len ||
            packet->len < (noinfo_len + sizeof(struct dmg_info)) ||
            hit_cnt > FO_SPELL_MAX_TARGET_HIT
        ) {
            return -1;
        }

        memcpy(&spell, packet, packet->len + sizeof(packet->len));

        out->len = spell.len;
        out->type = spell.type;
        out->origin_id = spell.origin_id;
        out->hit_cnt = hit_cnt;
        for (uint8_t i = 0; i < hit_cnt; i++) {
            out->dinfo[i] = spell.dinfo[i];
        }
    } else {
        /* wrong type */
        return -2;
    }

    if (gettimeofday(&out->timestamp, NULL) != 0) {
        fprintf(stderr, "[ERROR] gettimeofday returned an error: %s\n", strerror(errno));
        return -3;
    }

    /* Fix endianess where needed */
    for (uint8_t i = 0; i < out->hit_cnt; i++) {
        out->dinfo[i].inflicted_dmg = letosl(out->dinfo[i].inflicted_dmg);
        out->dinfo[i].remaining_hp = letosl(out->dinfo[i].remaining_hp);
    }

    return 0;
}

int
parse_packet_entity_info(const struct fopacket *packet, struct fopacket_entity_info *out)
{
    if (packet->type != FOPACKET_ENTITY_INFO) {
        return -2;
    }

    if (packet->len != sizeof(*out) - sizeof(out->len)) {
        return -1;
    }

    memcpy(out, packet, sizeof(*out));
    out->current_hp = letosl(out->current_hp);
    out->max_hp = letosl(out->max_hp);
    out->current_sp = letosl(out->current_sp);
    out->max_sp = letosl(out->max_sp);
    return 0;
}

int
parse_packet_friend_find(const struct fopacket *packet, struct fopacket_friend_find *out)
{
    uint32_t noplayer_len;
    uint8_t player_cnt;

    memset(out, 0, sizeof(*out));

    if (packet->type != FOPACKET_FRIEND_FIND) {
        return -2;
    }

    noplayer_len = sizeof(*out) - sizeof(out->len) - sizeof(struct player_info) * FO_FRIEND_MAX_PLAYERS;
    player_cnt = (uint8_t) (packet->len - noplayer_len) / sizeof(struct player_info);

    if (
        packet->len > out->len - sizeof(out->len) ||
        (packet->len % sizeof(struct player_info)) != noplayer_len ||
        packet->len < (noplayer_len + sizeof(struct player_info)) ||
        player_cnt > FO_FRIEND_MAX_PLAYERS
    ) {
        return -1;
    }

    memcpy(out, packet, packet->len + sizeof(packet->len));
    return 0;
}

int
handle_damage(struct fopacket *packet)
{
    int ret;
    struct fopacket_dmg packet_dmg;
    FOPDData *fopd_data = FOPDData::getInstance();

    ret = parse_packet_damage(packet, &packet_dmg);
    if (ret != 0) {
        printf("Damage parsing error\n");
        return ret;
    }

    for (uint8_t i = 0; i < packet_dmg.hit_cnt; i++) {
        printf(
            "[DEBUG] origin ID: %u\ttarget ID: %u\ttarget HP: %lu\tdmg done: %lu\n",
            packet_dmg.origin_id,
            packet_dmg.dinfo[i].target_id,
            packet_dmg.dinfo[i].remaining_hp,
            packet_dmg.dinfo[i].inflicted_dmg
        );
    }
    fopd_data->setTargetRemainingHealth(packet_dmg.dinfo[0].remaining_hp);
    fopd_data->pushToDamageQueue(packet_dmg);
    return 0;
}

int
handle_entity_info(struct fopacket *packet)
{
    int ret;
    struct fopacket_entity_info packet_info;

    ret = parse_packet_entity_info(packet, &packet_info);
    if (ret != 0) {
        return ret;
    }

    // Do stuff...
    return 0;
}

int
handle_friend_find(struct fopacket *packet)
{
    int ret;
    struct fopacket_friend_find packet_friend;
    FOPDData *fopd_data = FOPDData::getInstance();

    ret = parse_packet_friend_find(packet, &packet_friend);
    if (ret != 0) {
        return ret;
    }

    fopd_data->setFriendInfos(&packet_friend);
    return 0;
}
