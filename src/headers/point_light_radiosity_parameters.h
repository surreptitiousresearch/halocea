#pragma once
/* point_light_radiosity_parameters — radiosity contribution block of a point_light_definition
 * (0x20 bytes). Layout DB-verified against types_members. */

#include "real_rgb_color.h"

typedef struct point_light_radiosity_parameters
{
    float           intensity;   /* 0x00 */
    real_rgb_color  color;       /* 0x04 */
    int             unused[4];    /* 0x10 */
} point_light_radiosity_parameters; /* 0x20 */
