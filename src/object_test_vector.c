/* object_test_vector @ 0x83772E88 — test a ray against an object and its child/sibling chain,
 * recording the nearest hit in `collision`. Each object is rejected early if it is the ignored
 * object, flagged non-collidable, of a type not selected by `flags`, or whose bounding sphere the
 * ray misses. Selected objects are tested either against their physics hull (type bit 1 with flag
 * 0x400000) or their collision model; a closer hit overwrites the collision result (type 3 = object,
 * with the surface plane transformed into world space and the model's global material type resolved).
 * Children are recursed into; siblings are walked iteratively via the object's next-object link.
 *
 * collision model material type: damage_material.type from resistance.materials. */

#include <stdint.h>
#include "headers/collision_result.h"
#include "headers/object_type.h"
#include "headers/collision_model_instance.h"
#include "headers/collision_model_test_vector_result.h"
#include "headers/physics_instance.h"
#include "headers/physics_test_vector_result.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/damage_material.h"
#include "headers/collision_result_type.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/real_plane3d.h"
extern uint8_t fast_vector_intersects_sphere(const real_point3d *point, const real_vector3d *vector, const real_point3d *center, float radius);
extern uint8_t physics_instance_new(physics_instance *instance, int object_index);
extern uint8_t physics_test_vector(const physics_instance *instance, const real_point3d *point, const real_vector3d *vector, physics_test_vector_result *result);
extern uint8_t collision_model_instance_new(collision_model_instance *instance, int object_index);
extern uint8_t collision_model_test_vector(const collision_model_instance *instance, unsigned int flags, const real_point3d *point, const real_vector3d *vector, collision_model_test_vector_result *result);
extern real_plane3d *matrix4x3_transform_plane(const real_matrix4x3 *matrix, const real_plane3d *plane, real_plane3d *result);

uint8_t object_test_vector(int object_index, unsigned int flags, unsigned int bsp_flags,
                       const real_point3d *point, const real_vector3d *vector,
                       int ignore_object_index, collision_result *collision)
{
    int hit = 0;

    do
    {
        object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

        if ( object_index != ignore_object_index
          && (object->object.flags & (1u << _object_invisible_bit)) == 0
          && ((1 << (object->object.type + 8)) & flags) != 0
          && fast_vector_intersects_sphere(point, vector, &object->object.bounding_sphere_center,
                                           object->object.bounding_sphere_radius) )
        {
            if ( ((1 << object->object.type) & object_mask_vehicle) != 0 && (flags & 0x400000) != 0 )
            {
                physics_instance physics;
                physics_test_vector_result physics_result;
                if ( physics_instance_new(&physics, object_index)
                  && physics_test_vector(&physics, point, vector, &physics_result)
                  && collision->t > (double)physics_result.t )
                {
                    collision->t = physics_result.t;
                    collision->type = collision_result_object;
                    collision->material_type = -1;
                    collision->region_index = -1;
                    collision->node_index = -1;
                    collision->bsp_index = -1;
                    collision->plane.normal.n[0] = physics_result.plane.normal.n[0];
                    collision->plane.normal.n[1] = physics_result.plane.normal.n[1];
                    collision->plane.normal.n[2] = physics_result.plane.normal.n[2];
                    collision->plane.distance = physics_result.plane.distance;
                    collision->surface_index = -1;
                    collision->plane_designator = -1;
                    collision->flags = 0;
                    collision->breakable_surface_index = 0;
                    collision->material_index = -1;
                    collision->object_index = object_index;
                    hit = 1;
                }
            }
            else
            {
                collision_model_instance model_instance;
                collision_model_test_vector_result model_result;
                if ( collision_model_instance_new(&model_instance, object_index)
                  && collision_model_test_vector(&model_instance, bsp_flags, point, vector, &model_result)
                  && collision->t > (double)model_result.bsp_result.t )
                {
                    int plane_designator = model_result.bsp_result.plane_designator;
                    int16_t material_index = model_result.bsp_result.material_index;
                    int16_t material_type;

                    collision->t = model_result.bsp_result.t;
                    collision->type = collision_result_object;
                    matrix4x3_transform_plane(&model_instance.matrices[model_result.node_index],
                                              model_result.bsp_result.plane, &collision->plane);
                    if ( plane_designator < 0 )
                    {
                        collision->plane.normal.n[0] = -collision->plane.normal.n[0];
                        collision->plane.normal.n[1] = -collision->plane.normal.n[1];
                        collision->plane.normal.n[2] = -collision->plane.normal.n[2];
                        collision->plane.distance = -collision->plane.distance;
                    }

                    if ( material_index == -1 )
                        material_type = -1;
                    else
                    {
                        damage_material *materials =
                            (damage_material *)model_instance.model->resistance.materials.address;
                        material_type = materials[material_index].type;
                    }

                    collision->region_index = model_result.region_index;
                    collision->material_type = material_type;
                    collision->node_index = model_result.node_index;
                    collision->bsp_index = model_result.bsp_index;
                    collision->surface_index = model_result.bsp_result.surface_index;
                    collision->plane_designator = plane_designator;
                    collision->flags = model_result.bsp_result.flags;
                    collision->breakable_surface_index = model_result.bsp_result.breakable_surface_index;
                    collision->material_index = material_index;
                    collision->object_index = object_index;
                    hit = 1;
                }
            }

            {
                int child_object = object->object.first_child_object_index;
                if ( child_object != -1
                  && object_test_vector(child_object, flags, bsp_flags, point, vector,
                                        ignore_object_index, collision) )
                    hit = 1;
            }
        }

        object_index = object->object.next_object_index;
    }
    while ( object_index != -1 );

    return hit;
}
