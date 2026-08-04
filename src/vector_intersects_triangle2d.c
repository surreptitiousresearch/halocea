/* vector_intersects_triangle2d @0x836FEEB0 — test whether the 2D parametric segment point + t*vector
 * (t in [0,1]) intersects the triangle (p0,p1,p2). Clips the segment against the three edge half-planes,
 * shrinking a running [enter,exit] parameter interval; when the segment runs parallel to an edge and lies on
 * that edge's outside, or the interval becomes empty, there is no intersection (returns 0). Returns 1 if a
 * non-empty overlap survives all three edges.
 *
 * The decompiler recomputes each edge's cross-product numerator/denominator several times; factored to one
 * pair of locals per edge here (pure reads, identical values). Each edge: denom = vector x edge; numer =
 * (point - A) x edge; t = numer/denom, classifying denom<=0 as an exit bound and denom>0 as an enter bound. */

#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"

int vector_intersects_triangle2d(
    const real_point2d *point,
    const real_vector2d *vector,
    const real_point2d *p0,
    const real_point2d *p1,
    const real_point2d *p2)
{
    float enter = 0.0f;
    float exit = 1.0f;

    /* edge p0 -> p1 */
    {
        float edge_x = p1->n[0] - p0->n[0];
        float edge_y = p1->n[1] - p0->n[1];
        float denom = (vector->n[1] * edge_x) - (vector->n[0] * edge_y);
        float numer = ((point->n[0] - p0->n[0]) * edge_y) - (edge_x * (point->n[1] - p0->n[1]));
        if ( __fabs(denom) < 0.000099999997f )
        {
            if ( numer > 0.0f )
                return 0;
        }
        else
        {
            float t = numer / denom;
            if ( denom <= 0.0f )
            {
                if ( exit > t )
                    exit = t;
            }
            else if ( enter < t )
            {
                enter = t;
            }
            if ( enter > exit )
                return 0;
        }
    }

    /* edge p1 -> p2 */
    {
        float edge_x = p2->n[0] - p1->n[0];
        float edge_y = p2->n[1] - p1->n[1];
        float denom = (vector->n[1] * edge_x) - (vector->n[0] * edge_y);
        float numer = ((point->n[0] - p1->n[0]) * edge_y) - (edge_x * (point->n[1] - p1->n[1]));
        if ( __fabs(denom) < 0.000099999997f )
        {
            if ( numer > 0.0f )
                return 0;
        }
        else
        {
            float t = numer / denom;
            if ( denom <= 0.0f )
            {
                if ( exit > t )
                    exit = t;
            }
            else if ( enter < t )
            {
                enter = t;
            }
            if ( enter > exit )
                return 0;
        }
    }

    /* edge p2 -> p0 */
    {
        float edge_x = p0->n[0] - p2->n[0];
        float edge_y = p0->n[1] - p2->n[1];
        float denom = (vector->n[1] * edge_x) - (vector->n[0] * edge_y);
        float numer = ((point->n[0] - p2->n[0]) * edge_y) - (edge_x * (point->n[1] - p2->n[1]));
        if ( __fabs(denom) < 0.000099999997f )
        {
            if ( numer > 0.0f )
                return 0;
        }
        else
        {
            float t = numer / denom;
            if ( denom <= 0.0f )
            {
                if ( exit > t )
                    exit = t;
            }
            else if ( enter < t )
            {
                enter = t;
            }
            if ( enter > exit )
                return 0;
        }
    }

    return 1;
}
