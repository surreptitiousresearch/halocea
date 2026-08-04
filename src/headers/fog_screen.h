#pragma once

/* fog_screen — the "screen" (planar) fog sub-record of a fog tag, at fog tag body +0x84. 112-byte
 * record. DB-verified layout (types_members fog_screen). Referenced (as an opaque pointer) by
 * render_fog.screen and populated by structure_get_planar_fog. */

#include "tag_reference.h"

typedef struct fog_screen
{
    unsigned __int16 flags;                        /* 0x00 */
    __int16          layer_count;                  /* 0x02 */
    float            near_distance;                /* 0x04 */
    float            far_distance;                 /* 0x08 */
    float            near_density;                 /* 0x0C */
    float            far_density;                  /* 0x10 */
    float            start_distance_from_fog_plane;/* 0x14 */
    int              unused1[1];                   /* 0x18 */
    unsigned int     color;                        /* 0x1C */
    float            rotation_multiplier;          /* 0x20 */
    float            strafing_multiplier;          /* 0x24 */
    float            zoom_multiplier;              /* 0x28 */
    int              unused2[2];                    /* 0x2C */
    float            map_scale;                    /* 0x34 */
    tag_reference    map;                          /* 0x38 (56) */
    float            animation_period;             /* 0x48 (72) */
    float            animation_unused[1];          /* 0x4C */
    float            wind_velocity_lower_bound;     /* 0x50 */
    float            wind_velocity_upper_bound;     /* 0x54 */
    float            wind_period_lower_bound;       /* 0x58 */
    float            wind_period_upper_bound;       /* 0x5C */
    float            wind_acceleration_weight;      /* 0x60 */
    float            wind_perpendicular_weight;     /* 0x64 */
    int              wind_unused[2];                /* 0x68 */
} fog_screen;                                       /* 0x70 = 112 bytes */
