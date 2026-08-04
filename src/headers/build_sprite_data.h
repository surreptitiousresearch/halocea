#pragma once
/* build_sprite_data — accumulator state for one begin/end sprite-batch build (164 bytes, DB layout). */

#include "shader_effect.h"
#include "real_point3d.h"
#include "build_sprite_group.h"

typedef struct build_sprite_data
{
    int                 bitmap_group_index;   /* 0x00 */
    __int16             maximum_sprite_count; /* 0x04 */
    __int16             screen_space;         /* 0x06 */
    const shader_effect *shader;              /* 0x08 */
    __int16             sprite_count;         /* 0x0C */
    unsigned char _pad0[2]; /* db-verified padding */
    unsigned int        flags;                /* 0x10 */
    real_point3d        centroid;             /* 0x14 */
    __int16             group_count;          /* 0x20 */
    unsigned char _pad1[2]; /* db-verified padding */
    build_sprite_group  groups[8];            /* 0x24 */
} build_sprite_data;                            /* 164 bytes */
