#pragma once
/* model_marker_instance — one placement of a named model marker (DB-verified, 32 bytes). */

#include <stdint.h>
#include "real_point3d.h"
#include "real_quaternion.h"

typedef struct model_marker_instance
{
    uint8_t region_index;       /* 0x00 */
    uint8_t permutation_index;  /* 0x01 */
    uint8_t node_index;         /* 0x02 */
    uint8_t pad;                /* 0x03 */
    real_point3d translation;           /* 0x04 */
    real_quaternion rotation;           /* 0x10 */
} model_marker_instance;
