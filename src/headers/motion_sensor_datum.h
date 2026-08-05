#pragma once
/* motion_sensor_datum — per-sensor radar state: active blips, custom blips, orientation. */

#include <stdint.h>
#include "motion_sensor_blip.h"
#include "tiny_point2d.h"
#include "real_point2d.h"

typedef struct motion_sensor_datum
{
    motion_sensor_blip blips[16];                    /* 0x00 */
    tiny_point2d       custom_blips[16];             /* 0x40 */
    char               custom_blip_goal_indices[16]; /* 0x60 */
    real_point2d       reference_point;              /* 0x70 */
    int                num_blips_active;             /* 0x78 */
    float              yaw;                          /* 0x7C */
    uint8_t    custom_blips_active;          /* 0x80 */
} motion_sensor_datum; /* 132 bytes */
