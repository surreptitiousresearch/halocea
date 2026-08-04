/* vector_intersects_rectangle3d @0x836FEC98 — test whether the ray segment (point + t*vector, t in [0,1])
 * intersects an axis-aligned 3D box, using the slab method. For each axis: if the ray is (near) parallel, the
 * point must already lie within that axis's slab or there is no hit; otherwise the two slab-plane crossing
 * parameters tighten a running [t_min, t_max] entry/exit interval, and an empty interval means no hit. Finally
 * the interval must overlap the unit segment (t_max >= 0 and t_min <= 1). Returns 1 on intersection, else 0.
 *
 * DEVIATION: register-pun doubles are plain float math; the parallel-axis epsilon is 1e-4. */

#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rectangle3d.h"

extern double __fabs(double value);

int vector_intersects_rectangle3d(const real_point3d *point, const real_vector3d *vector,
        const real_rectangle3d *bounds)
{
    float t_min = -3.4028235e38f;
    float t_max = 3.4028235e38f;

    if ( __fabs(vector->n[0]) < 0.000099999997 )
    {
        if ( point->n[0] < bounds->n[0] || point->n[0] > bounds->n[1] )
            return 0;
    }
    else
    {
        float inverse = 1.0f / vector->n[0];
        float t_lo = (bounds->n[0] - point->n[0]) * inverse;
        float t_hi = (bounds->n[1] - point->n[0]) * inverse;
        if ( vector->n[0] <= 0.0f )
        {
            if ( t_hi > -3.4028235e38f ) t_min = t_hi;
            if ( t_lo < 3.4028235e38f )  t_max = t_lo;
        }
        else
        {
            if ( t_lo > -3.4028235e38f ) t_min = t_lo;
            if ( t_hi < 3.4028235e38f )  t_max = t_hi;
        }
        if ( t_min > t_max )
            return 0;
    }

    if ( __fabs(vector->n[1]) < 0.000099999997 )
    {
        if ( point->n[1] < bounds->n[2] || point->n[1] > bounds->n[3] )
            return 0;
    }
    else
    {
        float inverse = 1.0f / vector->n[1];
        float t_lo = (bounds->n[2] - point->n[1]) * inverse;
        float t_hi = (bounds->n[3] - point->n[1]) * inverse;
        if ( vector->n[1] <= 0.0f )
        {
            if ( t_min < t_hi ) t_min = t_hi;
            if ( t_max > t_lo ) t_max = t_lo;
        }
        else
        {
            if ( t_min < t_lo ) t_min = t_lo;
            if ( t_max > t_hi ) t_max = t_hi;
        }
        if ( t_min > t_max )
            return 0;
    }

    if ( __fabs(vector->n[2]) < 0.000099999997 )
    {
        if ( point->n[2] < bounds->n[4] || point->n[2] > bounds->n[5] )
            return 0;
    }
    else
    {
        float inverse = 1.0f / vector->n[2];
        float t_lo = (bounds->n[4] - point->n[2]) * inverse;
        float t_hi = (bounds->n[5] - point->n[2]) * inverse;
        if ( vector->n[2] <= 0.0f )
        {
            if ( t_min < t_hi ) t_min = t_hi;
            if ( t_max > t_lo ) t_max = t_lo;
        }
        else
        {
            if ( t_min < t_lo ) t_min = t_lo;
            if ( t_max > t_hi ) t_max = t_hi;
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
