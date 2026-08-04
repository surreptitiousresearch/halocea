#pragma once
/* path_destination — member of path_result.endpoint and actor_path_control_data.destination, 20 bytes.
 * Per structure dump from C:/halo/headers_ref/. */

#include "real_point3d.h"

typedef struct path_destination
{
    real_point3d point;           /* 0x00 */
    int surface_index;            /* 0x0C */
    float target_radius;          /* 0x10 */
} path_destination; /* 20 bytes */
