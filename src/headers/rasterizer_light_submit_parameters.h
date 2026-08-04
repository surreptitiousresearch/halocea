#pragma once
/* rasterizer_light_submit_parameters — one dynamic light submitted for the scene (56 bytes, DB layout). */

#include "point_light_definition.h"
#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_rgb_color.h"

typedef struct rasterizer_light_submit_parameters
{
    point_light_definition *definition; /* 0x00 */
    real_point3d            position;   /* 0x04 */
    real_vector3d           forward;    /* 0x10 */
    real_vector3d           up;         /* 0x1C */
    real_rgb_color          color;      /* 0x28 */
    float                   radius;     /* 0x34 */
} rasterizer_light_submit_parameters;    /* 56 bytes */
