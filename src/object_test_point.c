/* object_test_point @0x83772A50 — test a point against an object and its attachment chain. Walks the sibling list
 * starting at object_index; for each object that passes the ignore/disabled/type-mask and
 * bounding-sphere prefilter, runs a physics-instance or collision-model point test, and recurses into the
 * object's child chain. Returns 1 on the first containment. */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/object_type.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/collision_model_instance.h"
#include "headers/physics_instance.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


extern uint8_t physics_instance_new(physics_instance *instance, int object_index);
extern int physics_test_point(const physics_instance *instance, const real_point3d *point);
extern uint8_t collision_model_instance_new(collision_model_instance *instance, int object_index);
extern uint8_t collision_model_test_point(const collision_model_instance *instance, const real_point3d *point);

uint8_t object_test_point(int object_index, unsigned int flags, const real_point3d *point, int ignore_object_index)
{
    while ( 1 )
    {
        object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
        if ( object_index != ignore_object_index
          && (object->object.flags & (1u << _object_invisible_bit)) == 0 )
        {
            char object_type = (char)object->object.type;
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
                    if ( ((1 << object_type) & object_mask_vehicle) != 0 && (flags & 0x400000) != 0 )
                    {
                        physics_instance physics[2];
                        if ( !physics_instance_new(physics, object_index) )
                            tested = 0;
                        else
                            collided = physics_test_point(physics, point);
                    }
                    else
                    {
                        collision_model_instance instance;
                        if ( !collision_model_instance_new(&instance, object_index) )
                            tested = 0;
                        else
                            collided = collision_model_test_point(&instance, point);
                    }
                    if ( tested && collided )
                        return 1;
                }
            }
        }

        int child = object->object.first_child_object_index;
        if ( child != -1 && object_test_point(child, flags, point, ignore_object_index) )
            return 1;
        object_index = object->object.next_object_index;
        if ( object_index == -1 )
            return 0;
    }
}
