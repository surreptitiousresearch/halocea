#pragma once
/* rasterizer_distant_light_constants — one distant (directional) light's vertex-shader constants
 * (32 bytes, DB layout). */

#include "real_vector3d.h"
#include "real_rgb_color.h"

typedef struct rasterizer_distant_light_constants
{
    real_vector3d   forward;  /* 0x00 */
    float           pad0;     /* 0x0C */
    real_rgb_color  color;    /* 0x10 */
    float           pad1;     /* 0x1C */
} rasterizer_distant_light_constants;  /* 32 bytes */
