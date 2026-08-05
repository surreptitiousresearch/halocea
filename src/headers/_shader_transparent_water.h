#pragma once
/* _shader_transparent_water — water-shader-specific body (240 bytes, DB types_members-confirmed). */

#include <stdint.h>
#include "real_argb_color.h"
#include "tag_reference.h"
#include "tag_block.h"

typedef struct _shader_transparent_water
{
    uint16_t        flags;                          /* 0x00 — shader_transparent_water_flags bits */
    int16_t         type;                           /* 0x02 */
    int             unused1[8];                     /* 0x04 */
    tag_reference   base_map;                       /* 0x24 */
    int             unused2[4];                     /* 0x34 */
    real_argb_color view_perpendicular_tint_color;  /* 0x44 */
    real_argb_color view_parallel_tint_color;       /* 0x54 */
    int             unused3[4];                     /* 0x64 */
    tag_reference   reflection_map;                 /* 0x74 */
    int             unused4[4];                     /* 0x84 */
    float           ripple_animation_angle;         /* 0x94 */
    float           ripple_animation_velocity;      /* 0x98 */
    float           ripple_scale;                   /* 0x9C */
    tag_reference   ripple_maps;                    /* 0xA0 */
    int16_t         ripple_mipmap_levels;           /* 0xB0 */
    int16_t         pad2;                           /* 0xB2 */
    float           ripple_mipmap_fade_factor;      /* 0xB4 */
    float           ripple_mipmap_lod_bias;         /* 0xB8 */
    int             unused5[16];                    /* 0xBC */
    tag_block       ripples;                        /* 0xFC */
    int             unused6[4];                     /* 0x108 */
} _shader_transparent_water;                        /* 0x118 = 280 bytes */
