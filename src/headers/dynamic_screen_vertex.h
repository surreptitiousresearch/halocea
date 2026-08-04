#pragma once
/* dynamic_screen_vertex — a single vertex for the rasterizer's dynamic screen-space geometry
 * path (24 bytes): screen position, packed ARGB color, and texture coordinate. */

#include "real_point3d.h"
#include "real_point2d.h"

typedef struct dynamic_screen_vertex
{
    real_point3d position;    /* 0x00 */
    unsigned int color;       /* 0x0C */
    real_point2d texcoord;    /* 0x10 */
} dynamic_screen_vertex;      /* 24 bytes */
