/* vector_intersects_rectangle2d @0x836FEB18 — Liang-Barsky style clip test: does the segment from `point`
 * to `point + vector` cross the axis-aligned rectangle `bounds` (x0,x1,y0,y1)? For each axis, either the
 * segment must already lie within that axis's [min,max] range (if the vector's component on that axis is
 * ~0), or the entry/exit parametric t values against the two slab edges narrow the running [t_min,t_max]
 * intersection window. Returns false if the window ever becomes empty (t_min > t_max) or lies entirely
 * behind the origin/past the segment's end (t_max < 0 or t_min > 1). */

#include <stdint.h>
#include "headers/real_point2d.h"
#include "headers/real_vector2d.h"
#include "headers/real_rectangle2d.h"

extern float __fabs(float x);

uint8_t vector_intersects_rectangle2d(const real_point2d *point, const real_vector2d *vector, const real_rectangle2d *bounds)
{
    float t_min = -3.4028235e38f;
    float t_max = 3.4028235e38f;

    if ( __fabs(vector->n[0]) < 0.0001f )
    {
        if ( point->n[0] < bounds->n[0] || point->n[0] > bounds->n[1] )
            return 0;
    }
    else
    {
        float inv_x = 1.0f / vector->n[0];
        float t_x0 = (bounds->n[0] - point->n[0]) * inv_x;
        float t_x1 = (bounds->n[1] - point->n[0]) * inv_x;

        if ( vector->n[0] <= 0.0f )
        {
            if ( t_x1 > t_min )
                t_min = t_x1;
            if ( t_x0 < t_max )
                t_max = t_x0;
        }
        else
        {
            if ( t_x0 > t_min )
                t_min = t_x0;
            if ( t_x1 < t_max )
                t_max = t_x1;
        }

        if ( t_min > t_max )
            return 0;
    }

    if ( __fabs(vector->n[1]) < 0.0001f )
    {
        if ( point->n[1] < bounds->n[2] || point->n[1] > bounds->n[3] )
            return 0;
    }
    else
    {
        float inv_y = 1.0f / vector->n[1];
        float t_y0 = (bounds->n[2] - point->n[1]) * inv_y;
        float t_y1 = (bounds->n[3] - point->n[1]) * inv_y;

        if ( vector->n[1] <= 0.0f )
        {
            if ( t_min < t_y1 )
                t_min = t_y1;
            if ( t_max > t_y0 )
                t_max = t_y0;
        }
        else
        {
            if ( t_min < t_y0 )
                t_min = t_y0;
            if ( t_max > t_y1 )
                t_max = t_y1;
        }

        if ( t_min > t_max )
            return 0;
    }

    if ( t_max < 0.0f )
        return 0;

    if ( t_min > 1.0f )
        return 0;

    return 1;
}
