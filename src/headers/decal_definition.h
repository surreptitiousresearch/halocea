#pragma once
/* decal_definition — decal tag definition (268 bytes, DB types_members-confirmed layout). Bounds/animation
 * fields not yet load-bearing anywhere are left as sized opaque blobs. */

#include "tag_reference.h"
#include "real_rgb_color.h"
#include "shader_decal.h"

typedef struct decal_definition
{
    unsigned __int16 flags;                         /* 0x00 */
    __int16           type;                          /* 0x02 */
    __int16           layer;                          /* 0x04 */
    unsigned __int16 pad1;                            /* 0x06 */
    tag_reference     next_decal_in_chain;             /* 0x08 */
    float             radius_lower_bounds;             /* 0x18 */
    float             radius_upper_bounds;             /* 0x1C */
    int               unused1[3];                      /* 0x20 */
    float             intensity_lower_bounds;           /* 0x2C */
    float             intensity_upper_bounds;           /* 0x30 */
    real_rgb_color    color_lower_bounds;                /* 0x34 */
    real_rgb_color    color_upper_bounds;                 /* 0x40 */
    int               unused2[3];                          /* 0x4C */
    __int16           animation_loop_frame_index;           /* 0x58 */
    __int16           animation_speed;                        /* 0x5A */
    int               unused3[7];                              /* 0x5C */
    float             lifetime_lower_bounds;                    /* 0x78 */
    float             lifetime_upper_bounds;                     /* 0x7C */
    float             decay_time_lower_bounds;                    /* 0x80 */
    float             decay_time_upper_bounds;                     /* 0x84 */
    int               unused5[3];                                  /* 0x88 */
    shader_decal      shader;                                       /* 0x94 */
    float             runtime_maximum_sprite_extent;                 /* 0xFC */
    unsigned __int16  runtime_incremental_counter;                    /* 0x100 */
    unsigned __int16  pad2;                                            /* 0x102 */
    int               unused6[2];                                      /* 0x104 */
} decal_definition; /* 268 bytes */
