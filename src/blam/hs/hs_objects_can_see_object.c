/* hs_objects_can_see_object @0x837F7968 — return true if any unit in the object list can see the target
 * object within the given cone half-angle (degrees). For unit targets the head position is used; for other
 * objects the object's bounding-sphere center (object.bounding_sphere_center, +0xA0) is used. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_point3d.h"
#include "headers/object_type.h"
#include "headers/math_constants.h"
#include "headers/blam_data_globals.h"

extern int object_list_get_first(int object_list_index, int *reference_index);
extern int object_list_get_next(int object_list_index, int *reference_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern uint8_t unit_can_see_point(int unit_index, const real_point3d *point, float theta);

uint8_t hs_objects_can_see_object(int object_list_index, int object_index, float degrees)
{
    int iterator[2];
    int unit_index = object_list_get_first(object_list_index, iterator);
    if ( unit_index == -1 )
        return 0;

    while ( 1 )
    {
        if ( object_try_and_get_and_verify_type(unit_index, object_mask_unit) )
        {
            unsigned char can_see = 0;
            if ( object_index != -1 )
            {
                real_point3d target_point;
                if ( object_try_and_get_and_verify_type(object_index, object_mask_unit) )
                {
                    unit_get_head_position(object_index, &target_point);
                }
                else
                {
                    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
                    target_point.n[0] = object->object.bounding_sphere_center.n[0];   /* +0xA0 */
                    target_point.n[1] = object->object.bounding_sphere_center.n[1];
                    target_point.n[2] = object->object.bounding_sphere_center.n[2];
                }
                can_see = unit_can_see_point(unit_index, &target_point, degrees * DEG_TO_RAD);
            }
            if ( can_see )
                break;
        }
        unit_index = object_list_get_next(object_list_index, iterator);
        if ( unit_index == -1 )
            return 0;
    }
    return 1;
}
