#pragma once
/* sky_radiosity_light — the radiosity/lens-flare direction sub-block of a sky_light (sky tag).
 * 40 bytes, DB types_members-confirmed layout. */

#include "real_rgb_color.h"
#include "real_euler_angles2d.h"

typedef struct sky_radiosity_light
{
    unsigned int        flags;          /* 0x00 */
    real_rgb_color      color;          /* 0x04 */
    float               power;          /* 0x10 */
    float               test_distance;  /* 0x14 */
    int                 unused;         /* 0x18 */
    real_euler_angles2d direction;      /* 0x1C */
    float               diameter;       /* 0x24 */
} sky_radiosity_light;                  /* 40 bytes */
