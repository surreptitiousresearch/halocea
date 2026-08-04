#pragma once
/* model_marker_instance — one placement of a named model marker (DB-verified, 32 bytes). */

#include "real_point3d.h"
#include "real_quaternion.h"

typedef struct model_marker_instance
{
    unsigned __int8 region_index;       /* 0x00 */
    unsigned __int8 permutation_index;  /* 0x01 */
    unsigned __int8 node_index;         /* 0x02 */
    unsigned __int8 pad;                /* 0x03 */
    real_point3d translation;           /* 0x04 */
    real_quaternion rotation;           /* 0x10 */
} model_marker_instance;
