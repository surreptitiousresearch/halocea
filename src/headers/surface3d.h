#pragma once

/* surface3d — one bounding surface (half-space plane) of a convex hull, 28 bytes (DB types_members-confirmed
 * layout, resolved by convex_hull3d_test_point.c). */

#include <stdint.h>
#include "real_plane3d.h"

typedef struct surface3d
{
    uint8_t extant;     /* 0x00 */
    unsigned char _pad0[3]; /* db-verified padding */
    real_plane3d    plane;      /* 0x04 */
    int             edge_index; /* 0x14 */
    int             user_data;  /* 0x18 */
} surface3d; /* 28 bytes */
