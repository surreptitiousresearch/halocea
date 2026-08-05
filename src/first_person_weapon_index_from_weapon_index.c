/* first_person_weapon_index_from_weapon_index @0x8369E9A8 — the local player index whose unit currently
 * holds the given weapon object, or -1. Matches the weapon against each local player's unit's currently
 * equipped weapon (unit.current_weapon_index slot into unit.weapon_object_indices). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


extern int local_player_get_player_index(int16_t local_player_index);

int16_t first_person_weapon_index_from_weapon_index(int weapon_index)
{
    int16_t local_player_index;

    for ( local_player_index = 0; local_player_index < 2; local_player_index = (int16_t)(local_player_index + 1) )
    {
        int player_index = local_player_get_player_index(local_player_index);
        if ( player_index != -1 )
        {
            int unit_index = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index)->unit_index;
            if ( unit_index != -1 )
            {
                unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
                int16_t weapon_slot = unit->unit.current_weapon_index;
                if ( weapon_slot != -1 && weapon_index == unit->unit.weapon_object_indices[weapon_slot] )
                    return local_player_index;
            }
        }
    }
    return -1;
}
