#pragma once
/* shader_environment_reflection_properties — cube-map reflection / mirror block (DB, 116 bytes). */

#include "tag_reference.h"

typedef struct shader_environment_reflection_properties
{
    unsigned __int16 flags;                          /* 0x00 */
    __int16          type;                           /* 0x02 */
    float            lightmap_brightness_scale;      /* 0x04 */
    int              unused1[7];                      /* 0x08 */
    float            view_perpendicular_brightness;  /* 0x24 */
    float            view_parallel_brightness;       /* 0x28 */
    int              unused2[4];                      /* 0x2C */
    float            mirror_index_of_refraction;     /* 0x3C */
    float            mirror_depth;                    /* 0x40 */
    int              unused3[4];                      /* 0x44 */
    tag_reference    map;                             /* 0x54 — reflection cube map */
    int              unused4[4];                      /* 0x64 */
} shader_environment_reflection_properties;            /* 0x74 */
