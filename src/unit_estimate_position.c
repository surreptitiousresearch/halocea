/* unit_estimate_position @0x836C9380 — estimates where a unit's body is for AI/aiming. On-foot living
 * bipeds defer to biped_get_sight_position. For everything else (in a vehicle, dead, or a non-biped) it
 * builds the estimate by taking the unit's camera position and shifting it by the offset between the
 * supplied body_position and the unit's ground origin (using the vehicle's pathfinding surface point when
 * the unit rides a vehicle). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"


extern int vehicle_find_pathfinding_surface_index(int vehicle_index, real_point3d *pathfinding_point);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern void biped_get_sight_position(int biped_index, int16_t estimate_mode, const real_point3d *estimated_body_position, real_vector3d *desired_facing, const real_vector3d *desired_gun_offset, real_point3d *sight_position);

void unit_estimate_position(int unit_index, int16_t estimate_mode, const real_point3d *body_position,
                            real_vector3d *desired_facing, real_vector3d *desired_gun_offset,
                            real_point3d *estimated_position)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    int parent_index = unit->object.parent_object_index;

    if (parent_index != -1 || (unit->object.damage_flags & (1u << _object_dead_bit)) != 0
        || unit->object.type)
    {
        real_point3d ground;
        int have_ground = 0;
        if (!unit->object.type   /* unit is a biped */
            && parent_index != -1)
        {
            unit_datum *parent = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_index)->datum);
            if (parent->object.type == object_type_vehicle)   /* riding a vehicle */
                have_ground = vehicle_find_pathfinding_surface_index(parent_index, &ground) != -1;
        }
        if (!have_ground)
            object_get_origin(unit_index, &ground);

        unit_get_camera_position(unit_index, estimated_position);
        estimated_position->n[0] += body_position->n[0] - ground.n[0];
        estimated_position->n[1] += body_position->n[1] - ground.n[1];
        estimated_position->n[2] += body_position->n[2] - ground.n[2];
    }
    else
    {
        biped_get_sight_position(unit_index, estimate_mode, body_position, desired_facing,
                                 desired_gun_offset, estimated_position);
    }
}
