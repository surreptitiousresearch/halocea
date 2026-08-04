#pragma once
/* path_step — array element of path_result.steps[4], 16 bytes each.
 * Per structure dump from C:/halo/headers_ref/. */

#include "real_point3d.h"

typedef struct path_step
{
    int surface_index;            /* 0x00 */
    real_point3d point;           /* 0x04 */
} path_step; /* 16 bytes */
