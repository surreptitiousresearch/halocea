/* convex_hull3d_test_point @0x837FF5B8 — true if `point` lies inside (or on) every bounding half-space of
 * the convex hull's extant surfaces, i.e. is inside the hull. point_count/points/vertex_count/vertices/
 * edge_count/edges are unused (disasm-confirmed dead parameters — edge_count's register is even reused as
 * scratch for the loop bound before the loop starts).
 *
 * DEVIATION: the decompiler fabricated 19 extra trailing int parameters (a10..a27) and rendered the loop
 * bound as an opaque local; disasm_range(0x837FF5B8,0x837FF63C) shows the true parameter registers/stack
 * slot match the DB's 9-param prototype exactly, and the loop is a plain surface_count-bounded scan. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/vertex3d.h"
#include "headers/edge3d.h"
#include "headers/surface3d.h"
#include "headers/blam_data_globals.h"


uint8_t convex_hull3d_test_point(int16_t point_count, const real_point3d *points, int16_t vertex_count, const vertex3d *vertices, int16_t edge_count, const edge3d *edges, int16_t surface_count, const surface3d *surfaces, const real_point3d *point)
{
    for ( __int16 i = 0; i < surface_count; ++i )
    {
        const surface3d *surface = &surfaces[i];

        if ( surface->extant )
        {
            float distance = surface->plane.normal.n[0] * point->n[0]
                            + surface->plane.normal.n[1] * point->n[1]
                            + surface->plane.normal.n[2] * point->n[2]
                            - surface->plane.d;

            if ( distance > global_convex_hull3d_epsilon )
                return 0;
        }
    }

    return 1;
}
