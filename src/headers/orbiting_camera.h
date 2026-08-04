#pragma once
/* orbiting_camera — camera that orbits the focus point at a fixed distance. */

#include "real_euler_angles2d.h"

typedef struct orbiting_camera
{
    real_euler_angles2d orientation;    /* 0x0 */
    float               distance;       /* 0x8 */
} orbiting_camera;                      /* 12 bytes */
