#pragma once
/* light_volume_definition — a light-volume tag definition (332 bytes, DB layout). Only the fields touched by
 * the light-volume runtime are named; the `*_unused` blocks preserve offsets. */

#include "tag_reference.h"
#include "tag_block.h"

typedef struct light_volume_definition
{
    char             attachment_marker[32];           /* 0x00 */
    __int16          type;                             /* 0x20 */
    unsigned __int16 flags;                            /* 0x22 */
    int              unused1[4];                        /* 0x24 */
    float            near_fade_distance;               /* 0x34 */
    float            far_fade_distance;                /* 0x38 */
    float            perpendicular_brightness_scale;   /* 0x3C */
    float            parallel_brightness_scale;        /* 0x40 */
    __int16          brightness_scale_source;          /* 0x44 */
    unsigned __int16 pad1;                             /* 0x46 */
    int              fade_unused[5];                    /* 0x48 */
    tag_reference    map;                              /* 0x5C */
    __int16          sequence_index;                   /* 0x6C */
    __int16          count;                            /* 0x6E */
    int              map_unused[18];                    /* 0x70 */
    __int16          frame_animation_source;           /* 0xB8 */
    unsigned __int16 pad2;                             /* 0xBA */
    int              frame_animation_unused[9];         /* 0xBC */
    int              unused2[16];                        /* 0xE0 */
    tag_block        frames;                            /* 0x120 */
    int              unused3[8];                         /* 0x12C */
} light_volume_definition;                             /* 0x14C */
