#include "fopd/data_stream.h"

#include <stdint.h>

void
data_stream_push(struct data_stream *stream, uint32_t value)
{
    // a = a + ( v - a ) / (n + 1)
    stream->avg = stream->avg + ((double) value - stream->avg) / (double) (++stream->n);
}
