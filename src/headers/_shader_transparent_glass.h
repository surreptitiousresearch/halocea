#pragma once
/* _shader_transparent_glass — glass-shader-specific body (440 bytes, DB types_members-confirmed). */

#include <stdint.h>
#include "real_rgb_color.h"
#include "real_argb_color.h"
#include "tag_reference.h"

typedef struct _shader_transparent_glass
{
    uint16_t        flags;                                /* 0x00 — shader_transparent_glass_flags bits */
    int16_t         type;                                 /* 0x02 */
    int             unused1[10];                          /* 0x04 */
    real_rgb_color  tint_color;                           /* 0x2C */
    float           tint_map_scale;                       /* 0x38 */
    tag_reference   tint_map;                             /* 0x3C */
    int             unused2[5];                           /* 0x4C */
    uint16_t        reflection_flags;                     /* 0x60 */
    int16_t         reflection_type;                      /* 0x62 — shader_transparent_glass_reflection_type */
    real_argb_color reflection_view_perpendicular_color;  /* 0x64 */
    real_argb_color reflection_view_parallel_color;       /* 0x74 */
    tag_reference   reflection_map;                       /* 0x84 */
    float           reflection_bump_map_scale;            /* 0x94 */
    tag_reference   reflection_bump_map;                  /* 0x98 */
    int             unused3[32];                          /* 0xA8 */
    uint16_t        diffuse_flags;                        /* 0x128 */
    uint16_t        diffuse_pad;                          /* 0x12A */
    float           diffuse_map_scale;                    /* 0x12C */
    tag_reference   diffuse_map;                          /* 0x130 */
    float           diffuse_detail_map_scale;             /* 0x140 */
    tag_reference   diffuse_detail_map;                   /* 0x144 */
    int             diffuse_unused[7];                    /* 0x154 */
    uint16_t        specular_flags;                       /* 0x170 */
    uint16_t        specular_pad;                         /* 0x172 */
    float           specular_map_scale;                   /* 0x174 */
    tag_reference   specular_map;                         /* 0x178 */
    float           specular_detail_map_scale;            /* 0x188 */
    tag_reference   specular_detail_map;                  /* 0x18C */
    int             specular_unused[7];                   /* 0x19C */
} _shader_transparent_glass;                              /* 0x1B8 = 440 bytes */
