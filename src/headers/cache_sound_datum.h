#pragma once
/* cache_sound_datum — one entry in the sound cache request table (sound_cache.c).
 * 16 bytes, DB types_members-confirmed layout. */

typedef struct sound_permutation sound_permutation;

typedef struct cache_sound_datum
{
    __int16            identifier;               /* 0x00 */
    unsigned __int8    available;                /* 0x02 */
    unsigned __int8    postprocessed;            /* 0x03 */
    unsigned __int8    loading;                  /* 0x04 */
    unsigned __int8    software_reference_count; /* 0x05 */
    unsigned __int8    hardware_reference_count; /* 0x06 */
    unsigned char _pad0[1]; /* db-verified padding */
    __int16            request_index;            /* 0x08 */
    unsigned char _pad1[2]; /* db-verified padding */
    sound_permutation *sound;                    /* 0x0C */
} cache_sound_datum;                             /* 16 bytes */
