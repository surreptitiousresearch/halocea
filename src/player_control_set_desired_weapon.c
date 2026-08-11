/* player_control_set_desired_weapon @0x836E0480 — record the inventory slot a unit's controlling player
 * wants to switch to. Resolves the unit's controlling player (unit.player_index) and maps it to a local
 * player slot; if there is one, stores the desired weapon index into that player's control state. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/player_control_globals.h"
#include "headers/blam_data_globals.h"


void player_control_set_desired_weapon(int unit_index, int16_t desired_weapon_index)
{
    unit_datum *unit = (unit_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int controlling_player = unit->unit.player_index;

    int16_t local_player_index;
    if (controlling_player == -1)
        local_player_index = -1;
    else
        local_player_index =
            DATA_ARRAY_ELEMENT(player_data, player_datum, controlling_player)->local_player_index;

    if (local_player_index != -1)
        player_control_globals->players[local_player_index].desired_weapon_index = desired_weapon_index;
}
