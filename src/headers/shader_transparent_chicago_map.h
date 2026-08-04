#pragma once
/* shader_transparent_chicago_map — one map/stage of a transparent_chicago(_extended) shader
   (220 bytes; DB-verified against type 'shader_transparent_chicago_map', matches headers_ref). */

#include "real_vector2d.h"
#include "tag_reference.h"
#include "shader_texture_animation.h"

typedef struct shader_transparent_chicago_map
{
    unsigned __int16         flags;           /* 0x00 — bit1 = alpha replicate */
    __int16                  type;            /* 0x02 */
    int                      unused1[10];     /* 0x04 */
    __int16                  color_function;  /* 0x2C */
    __int16                  alpha_function;  /* 0x2E */
    int                      unused2[9];      /* 0x30 */
    real_vector2d            scale;           /* 0x54 (84) */
    real_vector2d            offset;          /* 0x5C (92) */
    float                    rotation;        /* 0x64 (100) */
    float                    mipmap_bias;     /* 0x68 (104) */
    tag_reference            map;             /* 0x6C (108) */
    int                      unused3[10];     /* 0x7C (124) */
    shader_texture_animation animation;       /* 0xA4 (164) */
} shader_transparent_chicago_map;             /* 220 bytes */
