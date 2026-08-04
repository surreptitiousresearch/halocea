#pragma once
/* rasterizer_lights — the frame's collected dynamic-light submissions (7176 bytes, DB layout). */

#include "rasterizer_light_submit_parameters.h"

// DB-verified via types_members rasterizer_lights (Blam spelling: rasterizer_lights_t)
typedef struct rasterizer_lights
{
    int                                 light_count;                     /* 0x0000 */
    rasterizer_light_submit_parameters  lights[128];                     /* 0x0004 */
    int                                 fixed_function_d3d_light_count;  /* 0x1C04 */
} rasterizer_lights_t;                                                   /* 7176 bytes */

extern rasterizer_lights_t rasterizer_lights;
