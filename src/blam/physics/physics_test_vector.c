/* physics_test_vector @ 0x837BDE60 — test a ray against an object's physics hull (a set of spheres,
 * the mass points). The ray is brought into the object's physics space; each mass-point sphere is
 * intersected; the nearest hit's surface plane (built from the returned normal and the local hit
 * point) is transformed back to world space. Returns 1 if any sphere was hit.
 *
 * Deviation: the decompiler scrambled sphere_test_vector3d's argument slots and split its output
 * across overlapping stack scratch; restored here from the function's real prototype
 * (center, radius, point, vector, out_t, out_normal) and the observed plane assembly. */

#include <stdint.h>
#include "headers/physics_instance.h"
#include "headers/physics_definition.h"
#include "headers/mass_point_definition.h"
#include "headers/physics_test_vector_result.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"

#include "headers/real_plane3d.h"
extern real_point3d *matrix4x3_inverse_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_inverse_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);
extern real_plane3d *matrix4x3_transform_plane(const real_matrix4x3 *matrix, const real_plane3d *plane, real_plane3d *result);
extern uint8_t sphere_test_vector3d(const real_point3d *center, float radius, const real_point3d *origin, const real_vector3d *direction, float *out_t, real_vector3d *out_normal);

uint8_t physics_test_vector(const physics_instance *instance, const real_point3d *point,
                        const real_vector3d *vector, physics_test_vector_result *result)
{
    const real_matrix4x3 *world_matrix = &instance->world_matrix;
    real_point3d local_point;
    real_vector3d local_vector;
    const physics_definition *physics;
    int found = 0;
    int i;

    result->t = 3.4028235e38f;
    matrix4x3_inverse_transform_point(world_matrix, point, &local_point);
    matrix4x3_inverse_transform_vector(world_matrix, vector, &local_vector);

    physics = instance->physics;
    for ( i = 0; i < physics->mass_points.count; ++i )
    {
        const mass_point_definition *mass_point =
            &((const mass_point_definition *)physics->mass_points.address)[i];
        const real_point3d *center = &mass_point->position;
        float radius = mass_point->radius;
        float hit_t;
        real_vector3d hit_normal;

        /* origin = local_point, direction = local_vector; the leading "point" slot is unused */
        if ( sphere_test_vector3d(center, radius, &local_point, &local_vector, &hit_t, &hit_normal)
          && result->t > (double)hit_t )
        {
            float hx = local_vector.n[0] * hit_t + local_point.n[0];
            float hy = local_vector.n[1] * hit_t + local_point.n[1];
            float hz = local_vector.n[2] * hit_t + local_point.n[2];

            result->t = hit_t;
            result->plane.normal.n[0] = hit_normal.n[0];
            result->plane.normal.n[1] = hit_normal.n[1];
            result->plane.normal.n[2] = hit_normal.n[2];
            result->plane.distance = result->plane.normal.n[0] * hx
                                   + (result->plane.normal.n[2] * hz + result->plane.normal.n[1] * hy);
            found = 1;
        }
        physics = instance->physics;
    }

    if ( found )
        matrix4x3_transform_plane(world_matrix, &result->plane, &result->plane);
    return found;
}
