#pragma once
#include "real_point2d.h"

typedef struct disc
{
    __int16      flags;           /* 0x00 */
    __int16      obstacle_index;  /* 0x02 */
    int          object_index;    /* 0x04 */
    real_point2d center;          /* 0x08 */
    float        radius;          /* 0x10 */
} disc;                           /* 20 bytes */
