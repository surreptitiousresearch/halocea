/* convex_hull2d @0x837FD590 — compute the 2D convex hull of `vertex_count` points by gift-wrapping (Jarvis
 * march), writing the hull's vertex indices into hull_vertex_indices and returning the hull vertex count. Returns
 * 0 if the point set is not genuinely 2-dimensional (points_dimension2d != 2). Starts from the
 * lexicographically-lowest point (min y, then min x) and repeatedly selects the next point by the smallest
 * counter-clockwise polar-angle increment relative to the running edge direction, accumulating the turn angle so
 * each step's search is anchored to the previous edge. It stops when the march returns to the start index (or, once
 * the hull has become non-degenerate, when it revisits the start position), then trims any leading run that the
 * wrap duplicated before the true closure.
 *
 * DEVIATION: the atan2 the decompiler rendered through a long-double register pun is atan2(dy, dx) (disasm
 * 0x837FD6EC-6FC: f1 = to->y - from->y, f2 = to->x - from->x). The angle-normalization while-loop adds 2*pi until
 * the increment is >= -1e-4. The uninitialized __int16 stack seed (v42) for the start/best index is provably
 * overwritten by the min-point scan (when vertex_count > 0) before use; modeled as 0. */

#include <stdint.h>
#include "headers/real_point2d.h"
#include <math.h>
#include "headers/math_constants.h"

extern int16_t points_dimension2d(int16_t count, const real_point2d *points);
/* atan2 provided by CRT via <math.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
extern double __fabs(double x);

int16_t convex_hull2d(int16_t vertex_count, const real_point2d *points, int16_t *hull_vertex_indices)
{
    int hull_count = 0;

    if ( points_dimension2d(vertex_count, points) != 2 )
        return hull_count;

    int16_t point_total = vertex_count;
    uint8_t hull_is_nondegenerate = 0;
    float accumulated_angle = 0.0f;

    /* find the lexicographically-lowest point (min y, then min x) as the march start */
    float min_y = 3.4028235e38f;
    float min_x = 3.4028235e38f;
    int16_t current_index = 0;   /* seeded from an uninitialized slot; overwritten below when count > 0 */
    if ( vertex_count > 0 )
    {
        int i = 0;
        do
        {
            const real_point2d *p = &points[i];
            if ( p->n[1] < (min_y - 0.000099999997f)
              || (p->n[1] < min_y && p->n[0] < (min_x + 0.000099999997f))
              || (p->n[1] < (min_y + 0.000099999997f) && p->n[0] < (min_x - 0.000099999997f)) )
            {
                min_x = p->n[0];
                current_index = (int16_t)i;
                min_y = p->n[1];
            }
            i = (int16_t)(i + 1);
        }
        while ( i < vertex_count );
    }

    int16_t best_index = 0;
    while ( 1 )
    {
        int hull_slot = (int16_t)hull_count;
        float min_angle_increment = 3.4028235e38f;
        if ( (int16_t)hull_count >= point_total )
            break;
        hull_count = (int16_t)(hull_count + 1);
        hull_vertex_indices[hull_slot] = current_index;

        if ( point_total > 0 )
        {
            const real_point2d *from = &points[current_index];
            for ( int candidate = 0; candidate < point_total; candidate = (int16_t)(candidate + 1) )
            {
                const real_point2d *to = &points[candidate];
                if ( to->n[0] != from->n[0] || to->n[1] != from->n[1] )
                {
                    float dx = to->n[0] - from->n[0];
                    float dy = to->n[1] - from->n[1];
                    float increment = ((float)atan2(dy, dx) - accumulated_angle);
                    while ( increment < -0.000099999997f )
                        increment = (increment + TWO_PI);
                    if ( increment < min_angle_increment )
                    {
                        min_angle_increment = increment;
                        best_index = (int16_t)candidate;
                    }
                }
            }
        }

        accumulated_angle = (min_angle_increment + accumulated_angle);
        current_index = best_index;

        if ( !hull_is_nondegenerate )
        {
            const real_point2d *best = &points[best_index];
            const real_point2d *start = &points[*hull_vertex_indices];
            if ( __fabs((best->n[0] - start->n[0])) >= 0.000099999997f
              || __fabs((best->n[1] - start->n[1])) >= 0.000099999997f )
                hull_is_nondegenerate = 1;
        }

        int start_index = *hull_vertex_indices;
        if ( best_index == start_index )
            return hull_count;
        if ( hull_is_nondegenerate )
        {
            const real_point2d *best = &points[best_index];
            const real_point2d *start = &points[start_index];
            if ( __fabs((best->n[0] - start->n[0])) < 0.000099999997f
              && __fabs((best->n[1] - start->n[1])) < 0.000099999997f )
                return hull_count;
        }
    }

    /* trim the leading run duplicated before the true closure */
    int16_t last = (int16_t)(hull_count - 1);
    int16_t scan = (int16_t)(hull_count - 2);
    if ( scan > 0 )
    {
        int16_t match = (int16_t)(hull_count - 2);
        while ( hull_vertex_indices[match] != hull_vertex_indices[last] )
        {
            scan = (int16_t)(scan - 1);
            match = scan;
            if ( scan <= 0 )
                return hull_count;
        }
        hull_count = (int16_t)(last - scan);
        if ( hull_count > 0 )
        {
            int dst = 0;
            for ( int src_offset = 0; src_offset < hull_count; src_offset = (int16_t)(src_offset + 1) )
            {
                hull_vertex_indices[dst] = hull_vertex_indices[src_offset + scan];
                dst = (int16_t)(src_offset + 1);
            }
        }
    }

    return hull_count;
}
