/* convex_hull2d_area @0x837FDC38 — area of a convex polygon via the shoelace/triangle-fan formula, summing
 * signed triangle areas (points[0], points[i-1], points[i]) and taking the absolute value. */

#include <stdint.h>
#include "headers/real_point2d.h"
#include <math.h>

float convex_hull2d_area(int16_t count, const real_point2d *points)
{
    float area = 0.0f;

    for ( int16_t i = 2; i < count; ++i )
    {
        const real_point2d *previous = &points[i - 1];
        const real_point2d *current = &points[i];

        area += 0.5f * ((current->n[1] - points->n[1]) * (previous->n[0] - points->n[0])
                       - (current->n[0] - points->n[0]) * (previous->n[1] - points->n[1]));
    }
    return fabsf(area);
}
