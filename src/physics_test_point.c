/* physics_test_point @0x837BDD88 — test whether a point lies inside any of a physics instance's mass-point
 * spheres. Transforms the point into the object's physics space, then checks it against each mass point's
 * bounding sphere. Returns 1 on first containment. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/physics_instance.h"
#include "headers/physics_definition.h"
#include "headers/mass_point_definition.h"

#include "headers/real_matrix4x3.h"
extern real_point3d *matrix4x3_inverse_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);

int physics_test_point(const physics_instance *instance, const real_point3d *point)
{
    real_point3d local;
    matrix4x3_inverse_transform_point(&instance->world_matrix, point, &local);

    const physics_definition *physics = instance->physics;
    int count = physics->mass_points.count;
    if ( count <= 0 )
        return 0;

    const mass_point_definition *mass_points =
        (const mass_point_definition *)physics->mass_points.address;
    for ( int i = 0; ; i = (int16_t)(i + 1) )
    {
        const mass_point_definition *mass_point = &mass_points[i];
        float dx = mass_point->position.n[0] - local.n[0];
        float dy = mass_point->position.n[1] - local.n[1];
        float dz = mass_point->position.n[2] - local.n[2];
        float radius = mass_point->radius;
        if ( ((dy * dy) + ((dx * dx) + (dz * dz)))
                <= (double)(radius * radius) )
            return 1;
        if ( (int16_t)(i + 1) >= count )
            return 0;
    }
}
