#include "fopd/network_stash.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


#define MAX_STASH_SIZE      65538


struct p_network_stash {
    uint8_t data[MAX_STASH_SIZE];   /* Data held by the stash */
    uint32_t data_len;              /* Current length of the buffer */
    uint32_t expected_len;          /* Expected length of the data buffer */
};

network_stash*
stash_create(void)
{
    return (network_stash*) calloc(1, sizeof(network_stash));
}

void
stash_destroy(network_stash *stash)
{
    free(stash);
}

int
stash_initialize(network_stash *stash, uint32_t total_len)
{
    stash_clear(stash);
    // printf("Stash init: %u\n", total_len);
    if (total_len == 0 || total_len > MAX_STASH_SIZE) {
        fprintf(stderr, "[ERROR] stash_init failed: %d > %d\n", total_len, MAX_STASH_SIZE);
        return -1;
    }

    stash->expected_len = total_len;
    return 0;
}

void
stash_clear(network_stash *stash)
{
    stash->data_len = 0;
    stash->expected_len = 0;
    memset(stash->data, 0, MAX_STASH_SIZE);
}

int
stash_push(network_stash *stash, const uint8_t *buf, uint32_t buf_size)
{
    // printf("Stash push: +%u => %u/%u\n", buf_size, stash->data_len + buf_size, stash->expected_len);

    if (
        buf_size > UINT16_MAX ||
        stash->data_len + buf_size > stash->expected_len
    ) {
        /* Overflow */
        return -1;
    }

    memcpy(&stash->data[stash->data_len], buf, buf_size);
    stash->data_len += buf_size;

    return 0;
}

uint32_t
stash_remaining_space(const network_stash *stash)
{
    /* Overflow should be never occur as it is protected by the "push" function */
    return stash->expected_len - stash->data_len;
}

bool
is_stash_full(const network_stash *stash)
{
    return stash->expected_len == stash->data_len;
}

int
stash_get_data(const network_stash *stash, uint8_t **buf, uint32_t *buf_size)
{
    if (buf == NULL || buf_size == NULL) {
        return -1;
    }

    if (!is_stash_initialized(stash) || !is_stash_full(stash)) {
        return -1;
    }

    *buf = (uint8_t *) malloc(stash->data_len);
    if (buf == NULL) {
        fprintf(stderr, "[ERROR] Memory allocation error\n");
        return -1;
    }

    memcpy(*buf, stash->data, stash->data_len);
    *buf_size = stash->data_len;
    return 0;
}

bool
is_stash_initialized(const network_stash *stash)
{
    return stash->expected_len != 0;
}
