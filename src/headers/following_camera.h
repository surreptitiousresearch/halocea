#pragma once
/* following_camera — third-person chase camera that follows the controlled unit/seat. */

#include <stdint.h>
#include "real_euler_angles2d.h"

typedef struct following_camera
{
    uint8_t     initialized;    /* 0x00 */
    uint8_t     confined;       /* 0x01 */
    uint8_t     crouched;       /* 0x02 */
    uint8_t     zoomed;         /* 0x03 */
    int16_t             zoom_level;     /* 0x04 */
    unsigned char _pad0[2]; /* db-verified padding */
    int                 unit_index;     /* 0x08 */
    int16_t             seat_index;     /* 0x0C */
    unsigned char _pad1[2]; /* db-verified padding */
    real_euler_angles2d facing_offset;  /* 0x10 */
    float               distance_scale; /* 0x18 */
} following_camera;                     /* 28 bytes */
