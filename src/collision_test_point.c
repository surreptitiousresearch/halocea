/* collision_test_point @0x83772BD8 — test whether a world point is inside solid geometry. First tests the
 * structure BSP; if the point is outside a leaf it counts as collided (returns 1). Otherwise, when object testing
 * is requested (flag 0x80), walks the leaf's collideable-object list: for each candidate that passes the type-mask
 * and bounding-sphere prefilter, runs either a physics-instance test (flag 0x400000 + physical object) or a
 * collision-model test, recursing into attached objects via object_test_point. Returns 1 on the first hit.
 *
 * Deviation: the object type bit is (1 << object_type); `(1 << (type+8)) & flags` reuses the same nibble shifted
 * into the flags' object-type-mask field, faithfully reproduced from the disassembly. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type.h"
#include "headers/structure_bsp.h"
#include "headers/structure_leaf.h"
#include "headers/bsp3d.h"
#include "headers/collision_model_instance.h"
#include "headers/physics_instance.h"
#include "headers/object_flags.h"
#include "headers/collision_test_flags.h"
#include "headers/blam_data_globals.h"


extern int bsp3d_test_point(const bsp3d *bsp, int node_index, const real_point3d *point);
extern int cluster_get_first_collideable_object(int *reference_index, int16_t cluster_index);
extern int cluster_get_next_collideable_object(int *reference_index);
extern uint8_t physics_instance_new(physics_instance *instance, int object_index);
extern int physics_test_point(const physics_instance *instance, const real_point3d *point);
extern uint8_t collision_model_instance_new(collision_model_instance *instance, int object_index);
extern uint8_t collision_model_test_point(const collision_model_instance *instance, const real_point3d *point);
extern uint8_t object_test_point(int object_index, unsigned int flags, const real_point3d *point, int ignore_object_index);

uint8_t collision_test_point(unsigned int flags, const real_point3d *point, int ignore_object_index)
{
    if ( (flags & (1u << _collision_test_structure_bit)) == 0
      && (flags & (1u << _collision_test_media_bit)) == 0
      && (flags & (1u << _collision_test_objects_bit)) == 0 )
        return 0;

    int leaf_index = bsp3d_test_point(global_bsp3d, 0, point);
    if ( leaf_index == -1 )
        return 1;
    if ( ((flags >> _collision_test_objects_bit) & 1) == 0 )
        return 0;

    int iterator[4];
    int object_index = cluster_get_first_collideable_object(
        iterator, ((structure_leaf *)global_structure_bsp->leaves.address)[leaf_index].cluster_index);
    if ( object_index == -1 )
        return 0;

    while ( 1 )
    {
        for ( int candidate = object_index; candidate != -1; )
        {
            object_datum *object =
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, candidate)->datum;
            char hit = 0;
            if ( candidate != ignore_object_index
              && (object->object.flags & (1u << _object_invisible_bit)) == 0 )
            {
                char object_type = object->object.type;
                if ( ((1 << (object_type + 8)) & flags) != 0 )
                {
                    float dz = object->object.bounding_sphere_center.n[2] - point->n[2];
                    float dx = object->object.bounding_sphere_center.n[0] - point->n[0];
                    float dy = object->object.bounding_sphere_center.n[1] - point->n[1];
                    float radius = object->object.bounding_sphere_radius;
                    if ( ((dy * dy) + ((dx * dx) + (dz * dz)))
                            <= (double)(radius * radius) )
                    {
                        unsigned __int8 collided;
                        char tested = 1;
                        if ( ((1 << object_type) & object_mask_vehicle) != 0 && (flags & (1u << _collision_test_use_vehicle_physics_bit)) != 0 )
                        {
                            physics_instance physics[2];
                            if ( !physics_instance_new(physics, candidate) )
                                tested = 0;
                            else
                                collided = physics_test_point(physics, point);
                        }
                        else
                        {
                            collision_model_instance instance;
                            if ( !collision_model_instance_new(&instance, candidate) )
                                tested = 0;
                            else
                                collided = collision_model_test_point(&instance, point);
                        }
                        if ( tested && collided )
                            hit = 1;
                    }
                }
            }

            if ( !hit )
            {
                int child = object->object.first_child_object_index;
                if ( child != -1 && object_test_point(child, flags, point, ignore_object_index) )
                    hit = 1;
            }
            if ( hit )
                return 1;
            candidate = object->object.next_object_index;
        }

        object_index = cluster_get_next_collideable_object(iterator);
        if ( object_index == -1 )
            return 0;
    }
}
