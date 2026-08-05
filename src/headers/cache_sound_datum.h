#pragma once
#include <stdint.h>
/* cache_sound_datum — one entry in the sound cache request table (sound_cache.c).
 * 16 bytes, DB types_members-confirmed layout. */

typedef struct sound_permutation sound_permutation;

typedef struct cache_sound_datum
{
    int16_t            identifier;               /* 0x00 */
    uint8_t    available;                /* 0x02 */
    uint8_t    postprocessed;            /* 0x03 */
    uint8_t    loading;                  /* 0x04 */
    uint8_t    software_reference_count; /* 0x05 */
    uint8_t    hardware_reference_count; /* 0x06 */
    unsigned char _pad0[1]; /* db-verified padding */
    int16_t            request_index;            /* 0x08 */
    unsigned char _pad1[2]; /* db-verified padding */
    sound_permutation *sound;                    /* 0x0C */
} cache_sound_datum;                             /* 16 bytes */
