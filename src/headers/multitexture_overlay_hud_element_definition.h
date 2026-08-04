#pragma once
/* multitexture_overlay_hud_element_definition — a HUD element that composites up to three texture maps
 * (with per-map scale/offset/blend) and animates them from scripted "functions" (heat/ammo/zoom effectors).
 * Layout from the database (480 bytes). */

#include "real_vector2d.h"
#include "tag_reference.h"
#include "tag_block.h"

typedef struct multitexture_overlay_hud_element_definition
{
    unsigned __int16 flags;                       /* 0x00 */
    __int16          type;                        /* 0x02 */
    __int16          framebuffer_blend_function;  /* 0x04 */
    unsigned __int16 pad1;                         /* 0x06 */
    int              unused1[8];                   /* 0x08 */
    unsigned __int16 map_flags[3];                 /* 0x28 */
    __int16          map_blending_function[2];     /* 0x2E — map0->1, map1->2 */
    __int16          pad2;                         /* 0x32 */
    real_vector2d    map_scale[3];                 /* 0x34 */
    real_vector2d    map_offset[3];                /* 0x4C */
    tag_reference    map[3];                       /* 0x64 */
    __int16          map_clamp[3];                 /* 0x94 */
    __int16          pad3;                         /* 0x9A */
    int              unused2[46];                  /* 0x9C */
    tag_block        functions;                    /* 0x154 — 220-byte effector records */
    int              unused3[32];                  /* 0x160 */
} multitexture_overlay_hud_element_definition; /* 0x1E0 = 480 bytes */
