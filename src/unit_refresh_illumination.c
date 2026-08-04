/* unit_refresh_illumination @0x836CC190 — recompute a unit's cached ambient and self illumination. If the
 * unit is riding another unit (parent is a unit object), it simply inherits the parent's ambient/self
 * illumination. Otherwise it samples the light environment at the unit's position and takes the object's
 * own self-illumination term. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/real_point3d.h"
#include "headers/real_rgb_color.h"
#include "headers/location.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void lights_illumination_at_point(const real_point3d *point, const location *location, real_rgb_color *color);
extern float real_rgb_color_brightness(const real_rgb_color *color);
extern float object_get_self_illumination(int object_index);

void unit_refresh_illumination(int unit_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    unit_datum *parent = object_try_and_get_and_verify_type(unit->object.parent_object_index, object_mask_unit);

    if (parent)
    {
        unit->unit.ambient_illumination = parent->unit.ambient_illumination;
        unit->unit.self_illumination = parent->unit.self_illumination;
    }
    else
    {
        real_rgb_color color;
        lights_illumination_at_point(&unit->object.position, &unit->object.location, &color);
        unit->unit.ambient_illumination = real_rgb_color_brightness(&color);
        unit->unit.self_illumination = object_get_self_illumination(unit_index);
    }
}
