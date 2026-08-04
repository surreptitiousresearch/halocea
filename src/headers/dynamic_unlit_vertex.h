#pragma once

#include "real_point3d.h"
#include "real_point2d.h"

/* dynamic_unlit_vertex — 24-byte position/color/uv vertex used by the dynamic
   (immediate-mode) unlit draw path, e.g. rasterizer_sun_glow_draw's screen quads. */
typedef struct dynamic_unlit_vertex
{
    real_point3d position;   /* 0x00 */
    unsigned int color;      /* 0x0C — packed ARGB */
    real_point2d texcoord;   /* 0x10 */
} dynamic_unlit_vertex;      /* 0x18 */
