#pragma once
/* camera_control — per-frame raw camera input deltas handed to a camera proc (camera.c). */

#include "real_euler_angles3d.h"
#include "real_vector3d.h"

typedef struct camera_control
{
    __int16             local_player_index; /* 0x00 */
    unsigned __int8     active;             /* 0x02 */
    unsigned char _pad0[1]; /* db-verified padding */
    float               seconds_elapsed;    /* 0x04 */
    real_euler_angles3d facing_delta;       /* 0x08 */
    real_vector3d       position_delta;     /* 0x14 */
    float               wheel_delta;        /* 0x20 */
} camera_control;                           /* 36 bytes */
