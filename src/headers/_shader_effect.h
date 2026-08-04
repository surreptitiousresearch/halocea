#pragma once
/* _shader_effect — secondary-map/blend-mode block of every shader_effect (140 bytes, DB layout). */

#include "tag_reference.h"
#include "shader_texture_animation.h"

typedef struct _shader_effect
{
    unsigned __int16         flags;                       /* 0x00 */
    __int16                  framebuffer_blend_function;  /* 0x02 */
    __int16                  framebuffer_fade_mode;       /* 0x04 */
    unsigned __int16         primary_map_flags;           /* 0x06 */
    int                      unused1[7];                  /* 0x08 */
    tag_reference             secondary_map;               /* 0x24 */
    __int16                  secondary_map_anchor;        /* 0x34 */
    unsigned __int16         secondary_map_flags;         /* 0x36 */
    shader_texture_animation secondary_map_animation;     /* 0x38 */
    float                    secondary_map_radius;        /* 0x70 */
    float                    zsprite_radius_scale;        /* 0x74 */
    int                      unused2[5];                  /* 0x78 */
} _shader_effect;                                          /* 140 bytes */
