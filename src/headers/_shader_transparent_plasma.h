#pragma once
/* _shader_transparent_plasma — plasma-shader-specific body (292 bytes, DB types_members-confirmed). */

#include <stdint.h>
#include "real_argb_color.h"
#include "real_vector3d.h"
#include "tag_reference.h"

typedef struct _shader_transparent_plasma
{
    uint16_t        flags;                                    /* 0x00 */
    int16_t         type;                                     /* 0x02 */
    int16_t         intensity_source;                         /* 0x04 */
    uint16_t        intensity_pad;                            /* 0x06 */
    float           intensity_exponent;                       /* 0x08 */
    int16_t         offset_source;                            /* 0x0C */
    uint16_t        offset_pad;                               /* 0x0E */
    float           offset_amount;                            /* 0x10 */
    float           offset_exponent;                          /* 0x14 */
    int             unused1[8];                               /* 0x18 */
    real_argb_color view_perpendicular_color;                 /* 0x38 */
    real_argb_color view_parallel_color;                      /* 0x48 */
    int16_t         tint_color_source;                        /* 0x58 */
    uint16_t        tint_color_pad;                           /* 0x5A */
    int             unused2[8];                               /* 0x5C */
    int16_t         thickness;                                /* 0x7C */
    uint16_t        pad;                                      /* 0x7E */
    int             unused3[4];                               /* 0x80 */
    float           mipmap_lod_bias;                          /* 0x90 */
    int             primary_noise_map_animation_unused;       /* 0x94 */
    float           primary_noise_map_animation_period;       /* 0x98 */
    real_vector3d   primary_noise_map_animation_direction;    /* 0x9C */
    float           primary_noise_map_scale;                  /* 0xA8 */
    tag_reference   primary_noise_map;                        /* 0xAC */
    int             unused4[8];                               /* 0xBC */
    int             secondary_noise_map_animation_unused;     /* 0xDC */
    float           secondary_noise_map_animation_period;     /* 0xE0 */
    real_vector3d   secondary_noise_map_animation_direction;  /* 0xE4 */
    float           secondary_noise_map_scale;                /* 0xF0 */
    tag_reference   secondary_noise_map;                      /* 0xF4 */
    int             unused5[8];                               /* 0x104 */
} _shader_transparent_plasma;                                 /* 0x124 = 292 bytes */
