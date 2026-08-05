#pragma once
/* sound_permutation — one playable sample variant within a pitch range (sound.c). 124 bytes. */

#include <stdint.h>
#include "tag_data.h"

typedef struct sound_permutation
{
    char          name[32];               /* 0x00 */
    float         skip_fraction;          /* 0x20 */
    float         gain;                   /* 0x24 */
    int16_t       duplicate_compression;  /* 0x28 */
    int16_t       next_permutation_index; /* 0x2A */
    int           cache_block_index;      /* 0x2C — -1 when not cached */
    void         *cache_base_address;     /* 0x30 */
    int           cache_tag_index;        /* 0x34 */
    int           unused0[1];             /* 0x38 */
    int           runtime_tag_index;      /* 0x3C */
    tag_data      samples;                /* 0x40 */
    tag_data      mouth_data;             /* 0x54 */
    tag_data      subtitle_data;          /* 0x68 */
} sound_permutation;                      /* 124 bytes */
