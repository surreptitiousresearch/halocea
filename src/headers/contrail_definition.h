#pragma once
/* contrail_definition — tag definition for a contrail effect. Full DB layout (0x144 bytes). */

#include "tag_reference.h"
#include "tag_block.h"
#include "shader_effect.h"

typedef struct contrail_definition
{
    unsigned __int16 flags;                             /* 0x00 — bit0 = fade in at head, bit1 = fade out at tail */
    unsigned __int16 scale_flags;                       /* 0x02 — 0x40 = scale U repeats by density, 0x80 = scale V repeats */
    float            point_generation_rate;             /* 0x04 */
    float            point_velocity_lower_bound;         /* 0x08 */
    float            point_velocity_upper_bound;         /* 0x0C */
    float            point_velocity_cone_angle;          /* 0x10 */
    float            point_inherited_velocity_fraction;  /* 0x14 */
    __int16          render_type;                        /* 0x18 — 0=vertical, 1/2=oriented, 3=none, 4=camera-facing */
    unsigned __int16 pad;                                /* 0x1A */
    float            texture_repeats_u;                  /* 0x1C */
    float            texture_repeats_v;                  /* 0x20 */
    float            texture_animation_u;                /* 0x24 */
    float            texture_animation_v;                /* 0x28 */
    float            frames_per_second;                  /* 0x2C */
    tag_reference    bitmap;                             /* 0x30 — sprite bitmap group (.index @ 0x3C) */
    __int16          first_sequence_index;               /* 0x40 */
    __int16          sequence_count;                     /* 0x42 */
    int              unused1[16];                        /* 0x44 */
    shader_effect    shader;                             /* 0x84 — contrail shader; effect.framebuffer_fade_mode at +0x2C (0xB0) */
    tag_block        states;                             /* 0x138 — contrail_state[] (104-byte elements) */
} contrail_definition;                                   /* 324 bytes */
