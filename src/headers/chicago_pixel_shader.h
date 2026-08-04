#pragma once

#include "shader_stage_cfg.h"

typedef struct D3DPixelShader D3DPixelShader;

/* Cached compiled chicago (fixed-function emulation) pixel shader (36 bytes). */
typedef struct chicago_pixel_shader
{
    shader_stage_cfg s1;    /* 0x00 */
    shader_stage_cfg s2;    /* 0x06 */
    shader_stage_cfg s3;    /* 0x0C */
    unsigned char _pad0[2]; /* db-verified padding */
    /* 0x12..0x13 padding */
    int is_cube;            /* 0x14 */
    int s4_arg;             /* 0x18 */
    int s4_op;              /* 0x1C */
    D3DPixelShader *shader; /* 0x20 */
} chicago_pixel_shader; /* 36 bytes */
