/* hs_unit_can_see_object @0x837F78A0 — HaloScript predicate: can `unit_index` see `object_index` within a
 * view cone of `degrees` (half-angle, converted to radians)? The target point is the object's head position
 * when it is a unit, otherwise its raw position (object floats +160/+164/+168). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/object_type.h"
#include "headers/math_constants.h"
#include "headers/blam_data_globals.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern uint8_t unit_can_see_point(int unit_index, const real_point3d *point, float theta);

unsigned __int8 hs_unit_can_see_object(int unit_index, int object_index, float degrees)
{
    if ( object_index == -1 )
        return 0;

    real_point3d target;
    if ( object_try_and_get_and_verify_type(object_index, object_mask_unit) )
    {
        unit_get_head_position(object_index, &target);
    }
    else
    {
        object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
        target.n[0] = object->object.bounding_sphere_center.n[0];
        target.n[1] = object->object.bounding_sphere_center.n[1];
        target.n[2] = object->object.bounding_sphere_center.n[2];
    }
    return unit_can_see_point(unit_index, &target, degrees * DEG_TO_RAD);
}
