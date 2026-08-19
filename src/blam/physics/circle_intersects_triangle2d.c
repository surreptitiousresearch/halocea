/* circle_intersects_triangle2d @0x836FF348 — test whether a circle overlaps a 2D triangle (p0,p1,p2):
 * for each of the triangle's 3 edges, if the circle's center is on the outside of that edge (per the
 * edge's 2D cross-product sign), it can only intersect the triangle by clipping that edge's capsule
 * ("pill") region — tested via point_in_pill2d. If the center is outside all 3 edges' capsules whenever
 * it's on the outside, there's no intersection; being on the inside of every edge (center inside the
 * triangle) also counts as intersecting.
 *
 * DEVIATION: the decompiler fabricates a phantom 6th parameter ("a6"); the DB's real 5-parameter
 * prototype (center, radius, p0, p1, p2) is ground truth. Cross-referencing the 3 edge tests (p1-p2,
 * p2-a6, a6-p1) against a real triangle's 3 edges (p1-p2, p2-p0, p0-p1) confirms a6 is p0 itself,
 * consistent with the rotated-but-still-closed edge order. */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

extern uint8_t point_in_pill2d(const real_point2d *point, const real_point2d *base, const real_vector2d *height, float width);

uint8_t circle_intersects_triangle2d(const real_point2d *center, float radius, const real_point2d *p0, const real_point2d *p1, const real_point2d *p2)
{
    uint8_t result = 1;
    real_vector2d edge;

    edge.n[0] = p2->n[0] - p1->n[0];
    edge.n[1] = p2->n[1] - p1->n[1];
    if ( (center->n[0] - p1->n[0]) * edge.n[1] - edge.n[0] * (center->n[1] - p1->n[1]) > 0.0f )
    {
        if ( point_in_pill2d(center, p1, &edge, radius) )
            return 1;
        result = 0;
    }

    edge.n[0] = p0->n[0] - p2->n[0];
    edge.n[1] = p0->n[1] - p2->n[1];
    if ( (center->n[0] - p2->n[0]) * edge.n[1] - edge.n[0] * (center->n[1] - p2->n[1]) < 0.0f )
    {
        if ( point_in_pill2d(center, p2, &edge, radius) )
            return 1;
        result = 0;
    }

    edge.n[0] = p1->n[0] - p0->n[0];
    edge.n[1] = p1->n[1] - p0->n[1];
    if ( (center->n[0] - p0->n[0]) * edge.n[1] - edge.n[0] * (center->n[1] - p0->n[1]) > 0.0f )
        return point_in_pill2d(center, p0, &edge, radius) != 0;

    return result;
}
