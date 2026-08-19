/* convex_hull2d_test_circle @0x837FDA30 — test whether a circle (center p, given radius) lies inside,
 * or crosses into, a convex 2D polygon given as an ordered vertex ring. For each directed edge the
 * signed area of the triangle (edge_start, edge_end, p) tells which side p is on; if p is on the outside
 * of any edge by more than radius (compared squared, scaled by the edge length squared), the circle is
 * entirely outside that edge and the function returns 0. If no edge rejects it, returns 1 (inside/overlap).
 * Degenerate (zero-length) edges are skipped. */

#include <stdint.h>
#include "headers/real_point2d.h"

uint8_t convex_hull2d_test_circle(int16_t count, const real_point2d *points, const real_point2d *p,
        float radius)
{
    int i;

    if ( count <= 0 )
        return 1;

    for ( i = 0; i < count; ++i )
    {
        int next = (i + 1 < count) ? i + 1 : 0;   /* wrap to first vertex on the last edge */
        const real_point2d *a = &points[i];
        const real_point2d *b = &points[next];

        float edge_x = b->n[0] - a->n[0];
        float edge_y = b->n[1] - a->n[1];
        float edge_len_sq = edge_x * edge_x + edge_y * edge_y;

        if ( edge_len_sq != 0.0f )
        {
            /* signed area * 2 of (a, b, p): positive means p is to the outside of this edge */
            float side = (p->n[0] - a->n[0]) * edge_y - edge_x * (p->n[1] - a->n[1]);

            if ( side > 0.0f && side * side > edge_len_sq * (radius * radius) )
                return 0;
        }
    }
    return 1;
}
