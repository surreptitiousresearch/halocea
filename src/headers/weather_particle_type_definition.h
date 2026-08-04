#pragma once
/* weather_particle_type_definition — per-type sub-block of a weather tag (604 bytes; DB-verified). */

#include "real_argb_color.h"
#include "tag_reference.h"
#include "shader_effect.h"

typedef struct weather_particle_type_definition
{
    char            name[32];                       /* 0x000 */
    unsigned int    flags;                          /* 0x020 */
    float           distance_fadein_start;          /* 0x024 */
    float           distance_fadein_end;            /* 0x028 */
    float           distance_fadeout_start;         /* 0x02C */
    float           distance_fadeout_end;           /* 0x030 */
    float           height_fadein_start;            /* 0x034 */
    float           height_fadein_end;              /* 0x038 */
    float           height_fadeout_start;           /* 0x03C */
    float           height_fadeout_end;             /* 0x040 */
    int             unused[24];                      /* 0x044 */
    float           particle_count_lower_bound;     /* 0x0A4 */
    float           particle_count_upper_bound;     /* 0x0A8 */
    tag_reference   physics;                        /* 0x0AC */
    int             unused2[4];                      /* 0x0BC */
    float           acceleration_lower_bound;       /* 0x0CC */
    float           acceleration_upper_bound;       /* 0x0D0 */
    float           acceleration_turning_rate;      /* 0x0D4 */
    float           acceleration_magnitude_rate;    /* 0x0D8 */
    int             unused3[8];                      /* 0x0DC */
    float           radius_lower_bound;             /* 0x0FC */
    float           radius_upper_bound;             /* 0x100 */
    float           animation_rate_lower_bound;     /* 0x104 */
    float           animation_rate_upper_bound;     /* 0x108 */
    float           rotation_rate_lower_bound;      /* 0x10C */
    float           rotation_rate_upper_bound;      /* 0x110 */
    int             unused4[8];                      /* 0x114 */
    real_argb_color color_lower_bound;              /* 0x134 */
    real_argb_color color_upper_bound;              /* 0x144 */
    float           runtime_oo_width;               /* 0x154 */
    int             unused5[15];                     /* 0x158 */
    tag_reference   bitmap;                         /* 0x194 */
    __int16         direction_render_type;          /* 0x1A4 */
    __int16         direction_source;               /* 0x1A6 */
    shader_effect   shader;                         /* 0x1A8 */
} weather_particle_type_definition;                 /* 604 bytes */
