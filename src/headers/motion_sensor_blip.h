#pragma once
/* motion_sensor_blip — one radar blip: packed position plus type/size classification. */

#include "tiny_point2d.h"

typedef struct motion_sensor_blip
{
    tiny_point2d pt;   /* 0x00 */
    char         type; /* 0x02 */
    char         size; /* 0x03 */
} motion_sensor_blip; /* 4 bytes */
