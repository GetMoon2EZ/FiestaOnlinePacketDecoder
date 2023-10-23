#ifndef __FOPD_DATA_STREAM_H__
#define __FOPD_DATA_STREAM_H__

/**
 * @file data_stream.h
 * @author GetMoon2EZ
 * @brief This data stream API is a tool to calculate mathematical statistics
 * on data streams. A data stream can be viewed as an unfinished (maybe infinite)
 * flow of data which may be received bit by bit over a long period of time.
 * The goal of this API is to obfuscate any mathematics to the user and to be as
 * space and time efficient as possible
 *
 * @version 0.1
 * @date 2023-10-23
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdint.h>

struct data_stream {
    double avg;     /* Average of the stream */
    uint32_t n;     /* Number of elements in the stream */
};

#define DATA_STREAM_INIT { 0.0, 0 }

/**
 * @brief Push a value to the data stream, updating the overall average.
 *
 * @param stream Stream to update
 * @param value Value to add to the average calculation
 *
 */
void data_stream_push(struct data_stream *stream, uint32_t value);

#endif // __FOPD_DATA_STREAM_H__
