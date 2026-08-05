#pragma once
/* effect_definition — the 'effe' tag body (64 bytes, DB layout from types_members). */

#include <stdint.h>
#include "tag_block.h"

typedef struct effect_definition
{
    unsigned int flags;                 /* 0x00 */
    int16_t      loop_start_index;      /* 0x04 */
    int16_t      loop_stop_index;       /* 0x06 */
    float        runtime_danger_radius; /* 0x08 */
    float        unused[7];             /* 0x0C */
    tag_block    locations;             /* 0x28 */
    tag_block    events;                /* 0x34 */
} effect_definition;                    /* 64 bytes */
