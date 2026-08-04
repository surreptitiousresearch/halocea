/* projectile_calculate_deceleration @0x8375AF10 — cache a projectile's flight deceleration profile from
 * its definition (air ranges at +464/+468/+476/+480, water ranges at +464/+468 shared with air's near
 * bound but a separate +476/+480 pair when underwater — object flags bit 0x10 selects "in water"): the
 * deceleration coefficient (604) is derived from (near_velocity^2 - far_velocity^2) / (2 * range), zero if
 * the range is degenerate; the reference range far bound (608) is always the air far range (+480); and the
 * time-to-decelerate-to-zero (600) is either the near range bound divided by the near velocity, or (596=1,
 * 600=0) when that near bound is already at or below the initial deceleration coefficient. */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"
#include "headers/projectile_definition.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


extern float projectile_calculate_deceleration_from_distances(projectile_definition *projectile_definition, float minimum_distance, float maximum_distance);
void projectile_calculate_deceleration(int projectile_index)
{
    projectile_datum *object = (projectile_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum;
    projectile_definition *definition = TAG_GET(projectile_definition, object->definition_index);

    float near_velocity = definition->projectile.initial_velocity;
    float far_velocity = definition->projectile.final_velocity;
    float near_bound;

    /* DEVIATION: verbatim inlined copy of projectile_calculate_deceleration_from_distances@0x83759DF0 (zero-xref), duplicated once per branch (water/air); donor's (minimum_distance, maximum_distance) params constant-fold to the water_* / air_* damage-distance pair per branch. */
    if ( object->object.flags & (1u << _object_wholly_under_media_bit) )
    {
        /* DEVIATION: verbatim inlined copy of projectile_calculate_deceleration_from_distances@0x83759DF0 (zero-xref) collapsed to a call; donor's (minimum_distance, maximum_distance) params fold to (water_minimum_damage_distance, water_maximum_damage_distance) here. */
        object->projectile.deceleration = projectile_calculate_deceleration_from_distances(
            definition, definition->projectile.water_minimum_damage_distance, definition->projectile.water_maximum_damage_distance);
        object->projectile.maximum_damage_distance = definition->projectile.water_maximum_damage_distance;
        near_bound = definition->projectile.water_minimum_damage_distance;
    }
    else
    {
        /* DEVIATION: verbatim inlined copy of projectile_calculate_deceleration_from_distances@0x83759DF0 (zero-xref) collapsed to a call; donor's (minimum_distance, maximum_distance) params fold to (air_minimum_damage_distance, air_maximum_damage_distance) here. */
        object->projectile.deceleration = projectile_calculate_deceleration_from_distances(
            definition, definition->projectile.air_minimum_damage_distance, definition->projectile.air_maximum_damage_distance);
        /* faithful: reference range far bound is always the air/water max at +480 */
        object->projectile.maximum_damage_distance = definition->projectile.water_maximum_damage_distance;
        near_bound = definition->projectile.air_minimum_damage_distance;
    }

    if ( near_bound <= 0.0f )
    {
        object->projectile.deceleration_timer_delta = 0.0f;
        object->projectile.deceleration_timer = 1.0f;
    }
    else
    {
        object->projectile.deceleration_timer_delta = near_bound / near_velocity;
    }
}
