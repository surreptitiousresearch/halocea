#pragma once
#include <stdint.h>
/* cache_copy_write_request — one pending write slot for the async map-copy worker (DB-verified: single
 * 16-bit sequence index; 0xFFFF/-1 means the slot is free). */

typedef struct cache_copy_write_request
{
    int16_t write_sequence_index;   /* 0x00 */
} cache_copy_write_request;         /* 2 bytes */
