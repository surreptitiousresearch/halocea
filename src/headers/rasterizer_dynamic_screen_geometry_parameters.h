#pragma once
/* rasterizer_dynamic_screen_geometry_parameters — texture/blend setup passed to the dynamic
 * screen-geometry draw path. Up to three texture maps, each with its own anchor, wrap mode,
 * offset, scale, texture scale, tint and fade. Layout from the database (139 bytes). */

#include "real_vector2d.h"
#include "real_point2d.h"
#include "real_rgb_color.h"
#include "real_argb_color.h"

#include "rasterizer_meter_parameters.h"

typedef struct bitmap_data bitmap_data; /* forward decl — only referenced by pointer here; include
                                          * headers/bitmap_data.h separately if the full layout is needed */

typedef struct rasterizer_dynamic_screen_geometry_parameters
{
    rasterizer_meter_parameters *meter_parameters;        /* 0x00 */
    real_vector2d *offset;                                /* 0x04 */
    unsigned __int8 map_anchor_screen[3];                 /* 0x08 */
    unsigned __int8 _pad0B[1];                            /* 0x0B */
    bitmap_data   *map[3];                                /* 0x0C */
    unsigned __int8 map_wrapped[3];                       /* 0x18 */
    unsigned __int8 _pad1B[1];                            /* 0x1B */
    real_point2d  *map_offset[3];                         /* 0x1C */
    real_vector2d  map_scale[3];                          /* 0x28 */
    real_vector2d  map_texture_scale[3];                  /* 0x40 */
    real_rgb_color *map_tint[3];                          /* 0x58 */
    real_argb_color plasma_fade;                          /* 0x64 */
    unsigned __int8 doing_plasma_effect;                  /* 0x74 */
    unsigned __int8 _pad75[3];                            /* 0x75 */
    float         *map_fade[3];                           /* 0x78 */
    __int16        map0_to_1_blend_function;              /* 0x84 */
    __int16        map1_to_2_blend_function;              /* 0x86 */
    __int16        framebuffer_blend_function;            /* 0x88 */
    unsigned __int8 point_sampled;                        /* 0x8A */
} rasterizer_dynamic_screen_geometry_parameters;
