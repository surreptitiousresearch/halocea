/* collision_test_vector_exit @0x83773970 — given a forward ray that entered an object (a previous collision of
 * type 3), find where the ray exits that same object. It sweeps a reversed ray from the ray's far endpoint back
 * toward its origin through the object's collision model; the first surface that hits is the exit face. The exit
 * result is filled with the surface's world-space plane (transformed by the hit node's matrix, flipped when the
 * plane designator is back-facing), the material, and all the surface/bsp indices, with the exit fraction t
 * converted back to the forward ray's parameterization (1 - reverse_t). Returns 1 if an exit was found. The exit
 * point is then placed along the forward ray at that t.
 *
 * DEVIATION: register-pun doubles are plain float math. */

#include <stdint.h>
#include "headers/collision_result.h"
#include "headers/collision_model_instance.h"
#include "headers/damage_material.h"
#include "headers/collision_model_test_vector_result.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_plane3d.h"
#include "headers/collision_result_type.h"

extern uint8_t collision_model_instance_new(collision_model_instance *instance, int object_index);
extern uint8_t collision_model_test_vector(const collision_model_instance *instance, unsigned int flags, const real_point3d *point, const real_vector3d *vector, collision_model_test_vector_result *result);
extern real_plane3d *matrix4x3_transform_plane(const real_matrix4x3 *matrix, const real_plane3d *plane, real_plane3d *result);

int collision_test_vector_exit(const collision_result *previous_collision, const real_point3d *point,
        const real_vector3d *vector, collision_result *exit_collision)
{
    exit_collision->type = -1;
    exit_collision->t = 3.4028235e38f;
    int found = 0;

    if ( previous_collision->type == collision_result_object )
    {
        real_point3d reverse_origin;
        real_vector3d reverse_vector;
        reverse_origin.n[0] = point->n[0] + vector->n[0];
        reverse_origin.n[1] = point->n[1] + vector->n[1];
        reverse_origin.n[2] = point->n[2] + vector->n[2];
        reverse_vector.n[0] = -vector->n[0];
        reverse_vector.n[1] = -vector->n[1];
        reverse_vector.n[2] = -vector->n[2];

        int object_index = previous_collision->object_index;
        collision_model_instance instance;
        if ( collision_model_instance_new(&instance, object_index) )
        {
            collision_model_test_vector_result result;
            if ( collision_model_test_vector(&instance, 1u, &reverse_origin, &reverse_vector, &result) )
            {
                exit_collision->type = collision_result_object;
                exit_collision->t = 1.0f - result.bsp_result.t;
                matrix4x3_transform_plane(&instance.matrices[result.node_index], result.bsp_result.plane,
                        &exit_collision->plane);
                if ( result.bsp_result.plane_designator < 0 )
                {
                    exit_collision->plane.n.n[0] = -exit_collision->plane.n.n[0];
                    exit_collision->plane.n.n[1] = -exit_collision->plane.n.n[1];
                    exit_collision->plane.n.n[2] = -exit_collision->plane.n.n[2];
                    exit_collision->plane.d = -exit_collision->plane.d;
                }

                if ( result.bsp_result.material_index == -1 )
                    exit_collision->material_type = -1;
                else
                    exit_collision->material_type =
                        ((damage_material *)instance.model->resistance.materials.address)
                          [result.bsp_result.material_index].type;

                exit_collision->region_index = result.region_index;
                exit_collision->node_index = result.node_index;
                exit_collision->bsp_index = result.bsp_index;
                exit_collision->surface_index = result.bsp_result.surface_index;
                exit_collision->object_index = previous_collision->object_index;
                exit_collision->plane_designator = result.bsp_result.plane_designator;
                exit_collision->flags = result.bsp_result.flags;
                exit_collision->breakable_surface_index = result.bsp_result.breakable_surface_index;
                exit_collision->material_index = result.bsp_result.material_index;
                found = 1;
            }
        }
    }

    if ( found )
    {
        exit_collision->point.n[0] = vector->n[0] * exit_collision->t + point->n[0];
        exit_collision->point.n[1] = vector->n[1] * exit_collision->t + point->n[1];
        exit_collision->point.n[2] = vector->n[2] * exit_collision->t + point->n[2];
    }
    return found;
}
