#pragma once
/* flying_camera — free-flying debug/orbit camera state (position + euler orientation + roll + FOV). */

#include "real_point3d.h"
#include "real_euler_angles2d.h"

typedef struct flying_camera
{
    real_point3d        position;       /* 0x00 */
    real_euler_angles2d orientation;    /* 0x0C */
    float               roll;           /* 0x14 */
    float               field_of_view;  /* 0x18 */
} flying_camera;                        /* 28 bytes */
