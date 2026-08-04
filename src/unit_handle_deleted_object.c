/* unit_handle_deleted_object @0x836CFEA8 — scrub every reference a unit holds to an object that is being
 * deleted. Clears the unit's various tracked object slots (recent-damage source +660, two extra tracked
 * objects +804/+808, held-item slots +792/+1036) when they name the deleted object, and walks the 4 weapon
 * slots (+760..+772); clearing a weapon slot also invalidates the current/desired weapon indices (+754/+756)
 * if they pointed at it. If the current weapon (+754) ends up invalid, it re-selects a desired weapon (+756)
 * from the remaining usable weapons, preferring the most-recently-used and stopping early on the first usable
 * weapon that either must be readied or lies past slot 0. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/maximum_weapons_per_unit.h"
#include "headers/blam_data_globals.h"

extern uint8_t unit_can_use_weapon(int unit_index, int weapon_index);
extern uint8_t weapon_must_be_readied(int weapon_index);

void unit_handle_deleted_object(int unit_index, int deleted_object_index)
{
    unit_datum *object =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    if ( object->unit.grenade_object_index == deleted_object_index )
        object->unit.grenade_object_index = -1;
    if ( object->unit.driver_object_index == deleted_object_index )
        object->unit.driver_object_index = -1;
    if ( object->unit.gunner_object_index == deleted_object_index )
        object->unit.gunner_object_index = -1;

    for ( int weapon_slot = 0; weapon_slot < MAXIMUM_WEAPONS_PER_UNIT; weapon_slot = (__int16)(weapon_slot + 1) )
    {
        if ( object->unit.weapon_object_indices[weapon_slot] == deleted_object_index )
        {
            object->unit.weapon_object_indices[weapon_slot] = -1;
            if ( weapon_slot == object->unit.desired_weapon_index )
                object->unit.desired_weapon_index = -1;
            if ( weapon_slot == object->unit.current_weapon_index )
                object->unit.current_weapon_index = -1;
        }
    }

    if ( (unsigned __int16)object->unit.current_weapon_index == 0xFFFF )
    {
        __int16 best_weapon_slot = -1;
        __int16 next_slot = 0;
        unit_datum *unit_object =
            (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
        int weapon_slot = 0;
        do
        {
            int weapon_index = unit_object->unit.weapon_object_indices[weapon_slot];
            if ( weapon_index != -1 && unit_can_use_weapon(unit_index, weapon_index) )
            {
                if ( best_weapon_slot == -1
                     || unit_object->unit.weapon_last_used_at_game_time[best_weapon_slot]
                        < unit_object->unit.weapon_last_used_at_game_time[weapon_slot] )
                    best_weapon_slot = next_slot;
                if ( weapon_must_be_readied(unit_object->unit.weapon_object_indices[weapon_slot]) || weapon_slot )
                    break;
            }
            next_slot = weapon_slot == MAXIMUM_WEAPONS_PER_UNIT - 1 ? 0 : weapon_slot + 1;
            weapon_slot = next_slot;
        }
        while ( next_slot );
        object->unit.desired_weapon_index = best_weapon_slot;
    }

    if ( object->unit.equipment_object_index == deleted_object_index )
        object->unit.equipment_object_index = -1;
    if ( object->unit.delayed_damage_attacker_object_index == deleted_object_index )
        object->unit.delayed_damage_attacker_object_index = -1;
}
