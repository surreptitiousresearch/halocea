#pragma once

#include "d3dx_effect_boundary.h"

/* 32-byte pixel-shader table entry. */
typedef struct rasterizer_dx9_shader
{
    ID3DXEffect *effect;     // 0x00
    unsigned int vshader9;   // 0x04
    unsigned int texture[4]; // 0x08
    unsigned int *constants; // 0x18  parameter-handle array
    char *filename;          // 0x1C
} rasterizer_dx9_shader;

#ifdef __cplusplus
extern "C" {
#endif

extern rasterizer_dx9_shader shader_table[123];
extern _D3DXMACRO global_effect_macros[3];
extern ID3DXEffectPool *global_effect_pool;
extern int dump_psh_to_binary;

#ifdef __cplusplus
}
#endif
