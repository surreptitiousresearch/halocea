#pragma once
/* following_camera — third-person chase camera that follows the controlled unit/seat. */

#include "real_euler_angles2d.h"

typedef struct following_camera
{
    unsigned __int8     initialized;    /* 0x00 */
    unsigned __int8     confined;       /* 0x01 */
    unsigned __int8     crouched;       /* 0x02 */
    unsigned __int8     zoomed;         /* 0x03 */
    __int16             zoom_level;     /* 0x04 */
    unsigned char _pad0[2]; /* db-verified padding */
    int                 unit_index;     /* 0x08 */
    __int16             seat_index;     /* 0x0C */
    unsigned char _pad1[2]; /* db-verified padding */
    real_euler_angles2d facing_offset;  /* 0x10 */
    float               distance_scale; /* 0x18 */
} following_camera;                     /* 28 bytes */
