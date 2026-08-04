#pragma once
/* rasterizer_model_lighting_constants — vertex-shader constant staging block for per-model lighting
 * (176 bytes, DB layout): 2 point lights, 2 distant lights, ambient color, padding. */

#include "rasterizer_point_light_constants.h"
#include "rasterizer_distant_light_constants.h"
#include "real_rgb_color.h"

typedef struct rasterizer_model_lighting_constants
{
    rasterizer_point_light_constants   point_lights[2];   /* 0x00 */
    rasterizer_distant_light_constants distant_lights[2]; /* 0x60 */
    real_rgb_color                     ambient;           /* 0xA0 */
    float                              pad;               /* 0xAC */
} rasterizer_model_lighting_constants;                     /* 176 bytes */
