/* object_get_features_in_sphere @0x83773E38 — walk an object and its attachment hierarchy (sibling chain via
 * object+0x114, children via object+0x118), and for each object whose bounding sphere overlaps the query sphere
 * and whose type bit is enabled in `flags`, emit collision features: bipeds (type 0) contribute a physics-pill
 * point feature; type-1/6/7/8 objects dispatch to the physics hull (when flag 0x400000 set and the object has a
 * physics phantom) or otherwise the collision model; types 2-5 contribute nothing beyond the recursion.
 *
 * Deviation: float-GPR-skip scrambles the Hex-Rays names. True register map (prologue): r3=flags, r4=object_index,
 * r5=center, f1=radius, f2=height, f3=width, r9=ignore_object_index, r10=features. The decompiler reused r7 (a
 * flag-mask temporary, `v24`) in several call slots that are really `features`; corrected from disasm. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_type.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/real_point3d.h"
#include "headers/collision_model_instance.h"
#include "headers/physics_instance.h"
#include "headers/collision_feature_list.h"
#include "headers/object_damage_flags.h"
#include "headers/biped_datum_flags.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/collision_test_flags.h"


extern void biped_get_physics_pill(int biped_index, real_point3d *base, float *height, float *width);
extern void collision_features_from_point(const real_point3d *point, float height, float width, int object_index, int surface_index, uint8_t flags, uint8_t breakable_surface_index, int16_t material_index, collision_feature_list *features);
extern uint8_t physics_instance_new(physics_instance *instance, int object_index);
extern uint8_t physics_get_features_in_sphere(const physics_instance *instance, const real_point3d *center, float radius, float height, float width, collision_feature_list *features);
extern uint8_t collision_model_instance_new(collision_model_instance *instance, int object_index);
extern uint8_t collision_model_get_features_in_sphere(const collision_model_instance *instance, const real_point3d *center, float radius, float height, float width, collision_feature_list *features);

void object_get_features_in_sphere(unsigned int flags, int object_index, const real_point3d *center, float radius,
                                   float height, float width, int ignore_object_index,
                                   collision_feature_list *features)
{
    do
    {
        object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
        biped_datum *biped = (biped_datum *)object;

        if ( object_index != ignore_object_index )
        {
            int object_flags = object->object.flags;
            if ( (object_flags & (1u << _object_invisible_bit)) == 0
              && (object_flags & (1u << _object_no_collisions_bit)) == 0
              && ((object->object.damage_flags & (1u << _object_dead_bit)) == 0 || object->object.type) )
            {
                float dx = (object->object.bounding_sphere_center.n[0] - center->n[0]);
                float dy = (object->object.bounding_sphere_center.n[1] - center->n[1]);
                float dz = (object->object.bounding_sphere_center.n[2] - center->n[2]);
                float reach = (object->object.bounding_sphere_radius + radius);

                if ( ((dy * dy) + ((dx * dx) + (dz * dz)))
                     <= (reach * reach) )
                {
                    char object_type = (char)object->object.type;
                    if ( (1 << (object_type + 8)) & flags )
                    {
                        unsigned int type = (uint16_t)object->object.type;
                        if ( type <= 8 )
                        {
                            if ( type == 1 )
                                goto dispatch_hull;
                            if ( type != 2 && type != 3 && type != 4 && type != 5 )
                            {
                                if ( !object->object.type )           /* type 0 — biped */
                                {
                                    if ( ((flags & (1u << _collision_test_skip_passthrough_bipeds_bit)) == 0 || (biped->biped.flags & (1u << _biped_movement_passes_through_bipeds_bit)) == 0)
                                      && (object->object.parent_object_index == -1
                                          || (uint16_t)biped->unit.parent_seat_index == 0xFFFF) )
                                    {
                                        real_point3d base;
                                        float pill_height;
                                        float pill_width;
                                        biped_get_physics_pill(object_index, &base, &pill_height, &pill_width);
                                        base.n[2] = base.n[2] + pill_height;
                                        collision_features_from_point(&base, (pill_height + height),
                                                                      (pill_width + width), object_index,
                                                                      -1, 0, 0xFF, -1, features);
                                    }
                                    goto recurse;
                                }
dispatch_hull:
                                if ( ((1 << object_type) & object_mask_vehicle) != 0 && (flags & (1u << _collision_test_use_vehicle_physics_bit)) != 0 )
                                {
                                    physics_instance physics;
                                    if ( physics_instance_new(&physics, object_index) )
                                        physics_get_features_in_sphere(&physics, center, radius, height, width, features);
                                }
                                else
                                {
                                    collision_model_instance model;
                                    if ( collision_model_instance_new(&model, object_index) )
                                        collision_model_get_features_in_sphere(&model, center, radius, height, width, features);
                                }
                            }
                        }
                    }

recurse:
                    {
                        int child = object->object.first_child_object_index;
                        if ( child != -1 )
                            object_get_features_in_sphere(flags, child, center, radius, height, width,
                                                          ignore_object_index, features);
                    }
                }
            }
        }

        object_index = object->object.next_object_index;
    }
    while ( object_index != -1 );
}
