#pragma once
/* sky_atmospheric_fog — fog parameters embedded in a sky tag. Layout from the database (32 bytes). */

#include "real_rgb_color.h"

typedef struct sky_atmospheric_fog
{
    real_rgb_color color;           /* 0x00 */
    float          unused[2];       /* 0x0C */
    float          maximum_density; /* 0x14 */
    float          z_near;          /* 0x18 */
    float          z_far;           /* 0x1C */
} sky_atmospheric_fog;
