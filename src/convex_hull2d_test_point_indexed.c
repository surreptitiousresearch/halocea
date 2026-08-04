/* convex_hull2d_test_point_indexed @0x837FDB88 — test whether `point` lies inside (or within epsilon of) a
 * convex polygon given as `count` vertices, indexed indirectly through `indices` into the shared `points`
 * array. Walks each edge (vertex[i] -> vertex[i+1], wrapping to 0) and rejects as soon as `point` is found to
 * be more than epsilon outside any edge's line (2D cross product test); returns 1 if it survives every edge.
 *
 * DEVIATION: the decompiler rendered the wraparound "next vertex index" computation as a 64-bit-pair
 * subtraction (`__PAIR64__` intrinsics) — this is the standard sign-trick idiom for `(i+1 < count) ? i+1 : 0`
 * (see firing_position_compare.c for the same class of bug); restored as the plain conditional it computes. */

#include <stdint.h>
#include "headers/real_point2d.h"

uint8_t convex_hull2d_test_point_indexed(int16_t count, const int16_t *indices, const real_point2d *points, const real_point2d *point, float epsilon)
{
    for ( __int16 i = 0; i < count; i++ )
    {
        const real_point2d *vertex = &points[indices[i]];
        __int16 next_index = (__int16)(i + 1) < count ? (__int16)(i + 1) : 0;
        const real_point2d *next_vertex = &points[indices[next_index]];

        float edge_x = next_vertex->n[0] - vertex->n[0];
        float edge_y = next_vertex->n[1] - vertex->n[1];
        float to_point_x = point->n[0] - vertex->n[0];
        float to_point_y = point->n[1] - vertex->n[1];

        float cross = edge_x * to_point_y - to_point_x * edge_y;
        if ( cross < -epsilon )
            return 0;
    }

    return 1;
}
