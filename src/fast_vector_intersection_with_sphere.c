/* fast_vector_intersection_with_sphere @0x836FDE88 */
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/ppc_intrinsics.h"

/* fast_vector_intersection_with_sphere — parametric distance along `vector` from `point` at which
 * the ray first enters the sphere (`center`, `radius`). Returns 0 when the origin is already inside
 * the sphere, FLT_MAX (3.4028235e38) when the ray points away or misses, otherwise the nearest
 * positive root of the quadratic. */
float fast_vector_intersection_with_sphere(const real_point3d *point, const real_vector3d *vector, const real_point3d *center, float radius)
{
    float dy = point->n[1] - center->n[1];
    float dz = point->n[2] - center->n[2];
    float dx = point->n[0] - center->n[0];
    float result = 0.0f;

    /* c-term of the quadratic: (|point-center|^2 - radius^2) */
    float c = -((radius * radius) - ((dx * dx) + ((dz * dz) + (dy * dy))));

    if (c >= 0.0f)
    {
        float vx = vector->n[0];
        float vy = vector->n[1];
        float vz = vector->n[2];
        /* b-term: dot(vector, point-center) */
        float b = (vx * dx) + ((vz * dz) + (vy * dy));
        float discriminant;

        if (b >= 0.0f
            || (discriminant = (b * b) - (((vx * vx) + ((vz * vz) + (vy * vy))) * c),
                discriminant <= 0.0f))
        {
            result = 3.4028235e38f;
        }
        else
        {
            result = (-b - __fsqrts(discriminant))
                   / ((vx * vx) + ((vz * vz) + (vy * vy)));
        }
    }

    return result;
}
