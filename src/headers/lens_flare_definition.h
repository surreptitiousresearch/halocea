#pragma once
/* lens_flare_definition — the lens-flare tag definition (240 bytes, DB types_members-confirmed). */

#include <stdint.h>
#include "tag_reference.h"
#include "tag_block.h"
#include "real_vector2d.h"

typedef struct lens_flare_definition
{
    float             falloff_angle;                 /* 0x00 */
    float             cutoff_angle;                  /* 0x04 */
    float             runtime_cosine_falloff_angle;  /* 0x08 */
    float             runtime_cosine_cutoff_angle;   /* 0x0C */
    float             occlusion_radius;              /* 0x10 */
    int16_t           occlusion_offset_direction;     /* 0x14 */
    uint16_t  occlusion_pad;                  /* 0x16 */
    float             near_fade_distance;             /* 0x18 */
    float             far_fade_distance;              /* 0x1C */
    tag_reference     primary_map;                    /* 0x20 */
    uint16_t  flags;                          /* 0x30 */
    uint16_t  pad;                            /* 0x32 */
    int               unused1[19];                    /* 0x34 */
    int16_t           corona_rotation_function;       /* 0x80 */
    uint16_t  corona_rotation_pad;             /* 0x82 */
    float             corona_rotation_function_scale;  /* 0x84 */
    int               unused2[6];                      /* 0x88 */
    real_vector2d     corona_radius_scale;              /* 0xA0 */
    int               unused3[7];                        /* 0xA8 */
    tag_block         reflections;                        /* 0xC4 */
    int               unused4[8];                          /* 0xD0 */
} lens_flare_definition; /* 240 bytes */
