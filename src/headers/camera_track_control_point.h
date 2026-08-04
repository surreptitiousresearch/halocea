#pragma once
/* camera_track_control_point — one keyframe of a camera-track animation (60 bytes). DB layout. */

#include "real_vector3d.h"
#include "real_quaternion.h"

typedef struct camera_track_control_point
{
    real_vector3d    position;      /* 0x00 */
    real_quaternion  orientation;   /* 0x0C */
    int              unused[8];     /* 0x1C */
} camera_track_control_point;       /* 60 bytes */
