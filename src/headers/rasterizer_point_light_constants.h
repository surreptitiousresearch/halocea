#pragma once
/* rasterizer_point_light_constants — one point light's vertex-shader constants (48 bytes, DB layout). */

#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_rgb_color.h"

typedef struct rasterizer_point_light_constants
{
    real_point3d    position;                   /* 0x00 */
    float           inverse_radius_squared;      /* 0x0C */
    real_vector3d   forward;                     /* 0x10 */
    float           spot_falloff_coefficient_A;  /* 0x1C */
    real_rgb_color  color;                       /* 0x20 */
    float           spot_falloff_coefficient_B;  /* 0x2C */
} rasterizer_point_light_constants;               /* 48 bytes */
