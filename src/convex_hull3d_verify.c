/* convex_hull3d_verify @0x837FF5B0 — retail stub: unconditionally returns 1. */

#include <stdint.h>
#include "headers/edge3d.h"
#include "headers/real_point3d.h"
#include "headers/surface3d.h"
#include "headers/vertex3d.h"

uint8_t convex_hull3d_verify(int16_t point_count, const real_point3d *points, int16_t vertex_count, const vertex3d *vertices, int16_t edge_count, const edge3d *edges, int16_t surface_count, const surface3d *surfaces)
{
    return 1;
}
