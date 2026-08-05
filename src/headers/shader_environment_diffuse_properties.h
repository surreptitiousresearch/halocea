#pragma once
/* shader_environment_diffuse_properties — base/detail/bump map block of an environment shader (DB, 276 bytes). */

#include <stdint.h>
#include "tag_reference.h"
#include "real_rgb_color.h"
#include "real_vector2d.h"

typedef struct shader_environment_diffuse_properties
{
    uint16_t flags;                       /* 0x00 */
    int16_t          type;                        /* 0x02 */
    int              unused1[6];                  /* 0x04 */
    tag_reference    base_map;                    /* 0x1C */
    int              unused2[6];                  /* 0x2C */
    int16_t          detail_map_function;         /* 0x44 */
    int16_t          detail_pad;                  /* 0x46 */
    float            primary_detail_map_scale;    /* 0x48 */
    tag_reference    primary_detail_map;          /* 0x4C */
    float            secondary_detail_map_scale;  /* 0x5C */
    tag_reference    secondary_detail_map;        /* 0x60 */
    int              unused3[6];                  /* 0x70 */
    int16_t          micro_detail_map_function;   /* 0x88 */
    int16_t          micro_detail_pad;            /* 0x8A */
    float            micro_detail_map_scale;      /* 0x8C */
    tag_reference    micro_detail_map;            /* 0x90 */
    real_rgb_color   material_color;              /* 0xA0 */
    int              unused4[3];                  /* 0xAC */
    float            bump_map_scale;              /* 0xB8 */
    tag_reference    bump_map;                    /* 0xBC */
    real_vector2d    runtime_bump_map_scale;      /* 0xCC */
    int              unused5[4];                  /* 0xD4 */
    int16_t          u_animation_function;        /* 0xE4 */
    int16_t          u_animation_pad;             /* 0xE6 */
    float            u_animation_period;          /* 0xE8 */
    float            u_animation_scale;           /* 0xEC */
    int16_t          v_animation_function;        /* 0xF0 */
    int16_t          v_animation_pad;             /* 0xF2 */
    float            v_animation_period;          /* 0xF4 */
    float            v_animation_scale;           /* 0xF8 */
    int              unused6[6];                  /* 0xFC */
} shader_environment_diffuse_properties;           /* 0x114 */
