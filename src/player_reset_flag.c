/* player_reset_flag @0x83807288 — drops the player's controlled unit's current weapon (networked, if the
 * unit isn't already in a vehicle — dword +4 of the unit object) and resets the given flag/weapon object. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"


extern void unit_drop_current_weapon_to_network(int unit_index, uint8_t immediate);
extern uint8_t unit_drop_current_weapon(int unit_index, uint8_t immediate);
extern void weapon_reset_flag(int weapon_index);

void player_reset_flag(int player_index, int weapon_index)
{
    int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;

    object_datum *unit = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    /* a nonzero datum_role means the unit is non-local (not owned by this machine) */
    if ( !unit->object.datum_role )
        unit_drop_current_weapon_to_network(unit_index, 1u);

    unit_drop_current_weapon(unit_index, 1u);
    weapon_reset_flag(weapon_index);
}
