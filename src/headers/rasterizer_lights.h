#pragma once
/* rasterizer_lights @ rasterizer_lights — per-frame submitted dynamic light set. light_count is touched at
 * begin-time; the per-light parameter array (56-byte entries) feeds the environment light passes. */

/* rasterizer_light_submit_parameters is defined in its own header; include it rather than re-declaring. */
#include "rasterizer_light_submit_parameters.h"

typedef struct rasterizer_lights_struct
{
    int                                light_count;                    /* 0x0000 */
    rasterizer_light_submit_parameters lights[128];                    /* 0x0004 */
    int                                fixed_function_d3d_light_count;  /* 0x1C04 */
} rasterizer_lights_struct;                                            /* 0x1C08 */

extern rasterizer_lights_struct rasterizer_lights;
