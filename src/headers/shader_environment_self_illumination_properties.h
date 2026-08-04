#pragma once
/* shader_environment_self_illumination_properties — primary/secondary/plasma self-illum block (DB, 252 bytes). */

#include "tag_reference.h"
#include "real_rgb_color.h"

typedef struct shader_environment_self_illumination_properties
{
    unsigned __int16 flags;                          /* 0x00 */
    __int16          type;                           /* 0x02 */
    int              unused1[6];                     /* 0x04 */
    real_rgb_color   primary_on_color;               /* 0x1C */
    real_rgb_color   primary_off_color;              /* 0x28 */
    __int16          primary_animation_function;     /* 0x34 */
    __int16          primary_animation_pad;          /* 0x36 */
    float            primary_animation_period;       /* 0x38 */
    float            primary_animation_phase;        /* 0x3C */
    int              unused2[6];                      /* 0x40 */
    real_rgb_color   secondary_on_color;             /* 0x58 */
    real_rgb_color   secondary_off_color;            /* 0x64 */
    __int16          secondary_animation_function;   /* 0x70 */
    __int16          secondary_animation_pad;        /* 0x72 */
    float            secondary_animation_period;     /* 0x74 */
    float            secondary_animation_phase;      /* 0x78 */
    int              unused3[6];                      /* 0x7C */
    real_rgb_color   plasma_on_color;                /* 0x94 */
    real_rgb_color   plasma_off_color;               /* 0xA0 */
    __int16          plasma_animation_function;      /* 0xAC */
    __int16          plasma_animation_pad;           /* 0xAE */
    float            plasma_animation_period;        /* 0xB0 */
    float            plasma_animation_phase;         /* 0xB4 */
    int              unused4[6];                      /* 0xB8 */
    float            map_scale;                       /* 0xD0 */
    tag_reference    map;                             /* 0xD4 */
    int              unused5[6];                      /* 0xE4 */
} shader_environment_self_illumination_properties;     /* 0xFC */
