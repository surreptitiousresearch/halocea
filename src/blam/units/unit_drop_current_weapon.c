/* unit_drop_current_weapon @ 0x836CF978 — make a unit drop the weapon in its current slot. Resolves the
 * current weapon (unit.current_weapon_index → unit.weapon_object_indices[slot]); refuses if there is no
 * weapon, or if cycling to the next slot would stay on the same weapon and the drop isn't immediate, or if
 * the weapon is already being deleted (its object flags bit 0x1), or if weapon_put_away fails. On success it
 * notifies the first-person view, detaches the item, clears the slot and current-weapon index, recomputes
 * the readied slot, and then either (for fireable weapons under the HCEX "AI drops half ammo" mod) halves the
 * dropped weapon's magazine-0 ammo and doubles its age, or (for non-fireable weapons in single-player)
 * deletes the object. Returns 1 on a
 * successful drop, 0 otherwise. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/weapon_datum.h"
#include "headers/game_connection.h"
#include "headers/object_flags.h"
#include "headers/first_person_weapon_message_type.h"
#include "headers/blam_data_globals.h"


extern int16_t unit_weapon_next_index(int unit_index, int16_t current_index, int16_t delta);
extern uint8_t weapon_put_away(int weapon_index, uint8_t immediate);
extern void first_person_weapon_message_from_unit(int unit_index, int16_t message_type);
extern void unit_drop_item(int unit_index, int item_index);
extern uint8_t weapon_can_be_fired(int weapon_index);
extern int16_t game_connection(void);
extern void object_delete(int object_index);

uint8_t unit_drop_current_weapon(int unit_index, uint8_t immediate)
{
    int weapon_index = -1;
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    int16_t current_slot = unit->unit.current_weapon_index;
    if ( current_slot != -1 )
        weapon_index = unit->unit.weapon_object_indices[current_slot];

    int16_t next_slot = unit_weapon_next_index(unit_index, unit->unit.current_weapon_index, 1);
    if ( weapon_index == -1 || (next_slot == unit->unit.current_weapon_index && !immediate) )
        return 0;

    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    if ( (weapon->object.flags & (1u << _object_invisible_bit)) != 0 || !weapon_put_away(weapon_index, immediate) )
        return 0;

    first_person_weapon_message_from_unit(unit_index, _first_person_weapon_message_drop);
    unit_drop_item(unit_index, weapon_index);
    unit->unit.weapon_object_indices[unit->unit.current_weapon_index] = -1;
    unit->unit.current_weapon_index = -1;
    unit->unit.desired_weapon_index = unit_weapon_next_index(unit_index, -1, 0);

    if ( weapon_can_be_fired(weapon_index) )
    {
        if ( hcex_ai_drop_wpn_half_ammo && unit->object.owner_player_index == -1 )
        {
            weapon_datum *dropped = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
            int16_t reserve_ammo = dropped->weapon.magazines[0].rounds_loaded;
            float doubled = (dropped->weapon.age * 2.0f);
            dropped->weapon.magazines[0].rounds_total /= 2;
            dropped->weapon.magazines[0].rounds_loaded = (reserve_ammo >> 1) + (reserve_ammo < 0 && (reserve_ammo & 1) != 0);
            if ( doubled < 0.0f )
                doubled = 0.0f;
            else if ( doubled > 1.0f )
                doubled = 1.0f;
            dropped->weapon.age = doubled;
        }
    }
    else if ( !game_connection() || game_connection() == _game_connection_film_playback )
    {
        object_delete(weapon_index);
        return 1;
    }
    return 1;
}
