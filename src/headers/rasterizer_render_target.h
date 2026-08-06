#pragma once

#include "d3d_render_boundary.h"

typedef struct rasterizer_render_target
{
    unsigned int width;   // 0x00
    unsigned int height;  // 0x04
    _D3DFORMAT format;    // 0x08
    D3DSurface *surface;  // 0x0C
    D3DTexture *texture;  // 0x10
} rasterizer_render_target;

#ifdef __cplusplus
extern "C" {
#endif

extern rasterizer_render_target global_render_targets[10];

#ifdef __cplusplus
}
#endif
