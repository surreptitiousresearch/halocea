#pragma once
#include <stdint.h>
#include "real_point2d.h"

typedef struct disc
{
    int16_t      flags;           /* 0x00 */
    int16_t      obstacle_index;  /* 0x02 */
    int          object_index;    /* 0x04 */
    real_point2d center;          /* 0x08 */
    float        radius;          /* 0x10 */
} disc;                           /* 20 bytes */
