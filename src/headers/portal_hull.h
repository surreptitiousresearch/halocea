#pragma once
/* portal_hull — a 2D screen-space convex hull (up to 256 vertices) produced when a portal/mirror polygon is
 * clipped against the view frustum. Layout from the database. */

#include "real_point2d.h"

typedef struct portal_hull
{
    __int16      vertex_count;       /* 0x000 */
    unsigned char _pad0[2]; /* db-verified padding */
    real_point2d vertices[256];      /* 0x004 */
} portal_hull;
