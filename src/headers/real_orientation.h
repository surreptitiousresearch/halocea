#pragma once
/* real_orientation — a node's local pose: rotation + translation + uniform scale (32 bytes).
 * Layout from the database. */

#include "real_quaternion.h"
#include "real_point3d.h"

typedef struct real_orientation
{
    real_quaternion rotation;     /* 0x00 */
    real_point3d    translation;  /* 0x10 */
    float           scale;        /* 0x1C */
} real_orientation;
