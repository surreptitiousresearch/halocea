#pragma once
/* firing_position_avoid_point — a sphere the AI should avoid when choosing firing positions. DB layout. */

#include "real_point3d.h"

typedef struct firing_position_avoid_point
{
    float        radius;   /* 0x0 */
    real_point3d point;    /* 0x4 */
} firing_position_avoid_point;   /* 0x10 (16 bytes) */
