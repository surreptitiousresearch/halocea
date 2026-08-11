/* game_engine_player_has_flag @0x83748DE8 — true if the given player's unit is carrying a CTF flag in any of
 * its 4 weapon inventory slots (unit+760, see unit_throw_grenade_begin.c for the same inventory offset).
 * False for no player, no unit, or an inventory with no flag weapon. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


extern uint8_t weapon_is_flag(int weapon_index);

uint8_t game_engine_player_has_flag(int player_index)
{
    if ( player_index == -1 )
        return 0;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int unit_index = player->unit_index;
    if ( unit_index == -1 )
        return 0;

    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int *inventory = unit->unit.weapon_object_indices;

    for ( int slot = 0; slot < 4; slot++ )
    {
        if ( inventory[slot] != -1 && weapon_is_flag(inventory[slot]) )
            return 1;
    }

    return 0;
}
