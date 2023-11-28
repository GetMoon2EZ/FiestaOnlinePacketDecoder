#include "fopd/player.h"

#include <stdio.h>
#include <stdlib.h>

#include <fopd/data_stream.h>


struct player *
player_init(void)
{
    struct player *p;

    p = (struct player *) calloc(1, sizeof(*p));
    if (p == NULL) {
        fprintf(stderr, "[ERROR] calloc returned NULL: %s", strerror(errno));
    }

    p->dps_avg = DATA_STREAM_INIT;
    p->dps_if = DATA_STREAM_INIT;

    return p;
}
