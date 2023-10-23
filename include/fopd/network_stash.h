#ifndef __NETWORK_STASH_H__
#define __NETWORK_STASH_H__

/**
 * @file network_stash.h
 * @author GetMoon2EZ
 * @brief This file provides an API for a simple network stashing
 * mechanism. It is intended to be used whenever receiving incomplete
 * data from the game's TCP stream, storing the bit of data that has been
 * received while waiting for the remaining data.
 * XXX: Stash is not that good of a name for this kind of storing
 * @version 0.1
 * @date 2023-10-22
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <stdint.h>


typedef struct p_network_stash network_stash;

/**
 * @brief Create a new stash
 *
 * @return network_stash* Pointer to the newly created stash.
 */
network_stash* stash_create(void);

/**
 * @brief Destroy a stash.
 *
 * @param stash Stash to destroy (must have be created using stash_create)
 */
void stash_destroy(network_stash *stash);


/**
 * @brief Initialize a network stash which will expect "total_len" elements
 *
 * @param stash Stash to initialize
 * @param total_len Total length of the data stash (see stash_push)
 * @return int -1 on error, 0 otherwise
 */
int stash_initialize(network_stash *stash, uint32_t total_len);

/**
 * @brief Clear a stash, removing all data which is stored in it.
 * stash_initialize must be used prior to reusing this stash.
 *
 * @param stash Stash to clear
 */
void stash_clear(network_stash *stash);

/**
 * @brief Push data in the stash, the data will be stored after any data
 * which is already in the stash (much like a queue).
 *
 * @param stash Stash to push data in
 * @param buf Data to push in the stash
 * @param buf_size Size of the data to push in the stash
 * @return int -1 on error, 0 otherwise
 */
int stash_push(network_stash *stash, const uint8_t *buf, uint32_t buf_size);

/**
 * @brief Get the remaining space in the stash, the remaining space is the
 * total length provided in stash_initialize minus all of the data that has
 * been pushed.
 *
 * @param stash Stash to get the remaining space for
 * @return uint32_t Remaining space in the stash
 */
uint32_t stash_remaining_space(const network_stash *stash);

/**
 * @brief Check if a stash is full (ie: no more data can be pushed in)
 *
 * @param stash Stash to check the fullness of
 * @return true if the stash is full
 * @return false if the stash is not full
 *
 * Note: An uninitialized stash is considered full
 */
bool is_stash_full(const network_stash *stash);

/**
 * @brief Get a copy of the data from the stash.
 * This function should only be called when the stash if full.
 *
 * @param stash Stash to get the data from
 * @param buf Buffer which will contain the stashed data on success
 * @param buf_len Length of the returned buffer
 * @return int -1 if an error occurred (stash not full, not initialized etc...)
 *              0 otherwise
 * Note: The returned buffer must be freed by the caller.
 */
int stash_get_data(const network_stash *stash, uint8_t **buf, uint32_t *buf_size);

/**
 * @brief Check if a stash is initialized
 *
 * @param stash Stash to check the status of
 * @return true if the stash is initialized
 * @return false if the stash in not initialized
 */
bool is_stash_initialized(const network_stash *stash);

#endif // __NETWORK_STASH_H__
