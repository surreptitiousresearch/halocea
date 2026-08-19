/* ai_handle_bump @0x836E8E10 — mutual AI-awareness notification when a biped bumps into a
 * biped/vehicle: resolve the "blame" object (the bumped object's driver, unit +804, when it has one;
 * otherwise the object itself), and unless its word +180 is already set (a gate not otherwise named), make
 * each side's controlling actor (unit +500, per unit_camera_get.c's established field) aware of the other
 * side via its prop system. `old_velocity` is unused here (kept for signature parity with the DB
 * prototype). */

#include <stdint.h>
#include "headers/unit_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_vector3d.h"
#include "headers/object_type.h"
#include "headers/ai_unit_effect.h"
#include "headers/blam_data_globals.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int prop_get_base_by_unit_index(int actor_index, int unit_index, uint8_t acquire, uint8_t refresh_upon_acquisition);
extern void actor_handle_unit_effect(int actor_index, int prop_index, int16_t effect_type);

void ai_handle_bump(int biped_index, int object_index, const real_vector3d *old_velocity)
{
    if ( object_index == -1 )
        return;

    unit_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_unit);
    if ( !object )
        return;

    int blame_unit_index = object->unit.driver_object_index;
    if ( blame_unit_index == -1 )
    {
        if ( object_index == -1 )
            return;
        blame_unit_index = object_index;
    }

    unit_datum *blame_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, blame_unit_index)->datum;

    if ( blame_object->object.type )
        return;

    unit_datum *biped_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;

    int biped_controlling_actor = biped_object->unit.actor_index;
    if ( biped_controlling_actor != -1 )
    {
        int prop_index = prop_get_base_by_unit_index(biped_controlling_actor, blame_unit_index, 1u, 0);
        if ( prop_index != -1 )
            actor_handle_unit_effect(biped_object->unit.actor_index, prop_index, _ai_unit_effect_bump);
    }

    int blame_controlling_actor = blame_object->unit.actor_index;
    if ( blame_controlling_actor != -1 )
    {
        int prop_index = prop_get_base_by_unit_index(blame_controlling_actor, biped_index, 1u, 0);
        if ( prop_index != -1 )
            actor_handle_unit_effect(blame_object->unit.actor_index, prop_index, _ai_unit_effect_bump);
    }
}
