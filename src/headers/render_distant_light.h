#pragma once
/* render_distant_light — a directional light contribution baked into an object's render lighting. */

#include "real_rgb_color.h"
#include "real_vector3d.h"

typedef struct render_distant_light
{
    real_rgb_color color;     /* 0x00 */
    real_vector3d  direction; /* 0x0C */
} render_distant_light;        /* 24 bytes */
