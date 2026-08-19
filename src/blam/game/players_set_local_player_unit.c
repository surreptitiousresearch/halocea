/* players_set_local_player_unit @0x836A8328 — bind a local player to a unit. Releases the player's previous
 * unit (clears its unit.player_index and marks it not actively controlled), then takes control of the new
 * unit (marks it actively controlled, writes the player's global index into unit.player_index). The player
 * datum's unit fields are updated (unit_index = unit, dead_unit_index = -1) and player_control is told about
 * the swap. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/players_global_data.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern int player_control_get_unit_index(int16_t local_player_index);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);
extern void player_control_new_unit(int16_t local_player_index, int unit_index);

void players_set_local_player_unit(int16_t local_player_index, int new_unit_index)
{
    int old_unit_index = player_control_get_unit_index(local_player_index);
    if ( old_unit_index != -1 )
    {
        unit_datum *old_unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, old_unit_index)->datum);
        old_unit->unit.player_index = -1;
        unit_set_actively_controlled(old_unit_index, 0);
    }

    if ( new_unit_index != -1 )
    {
        unit_datum *new_unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, new_unit_index)->datum);
        unit_set_actively_controlled(new_unit_index, 1u);
        int global_player_index = (local_player_index == -1) ? -1 : players_globals->local_player_indices[local_player_index];
        new_unit->unit.player_index = global_player_index;
    }

    int player_index = (local_player_index == -1) ? -1 : players_globals->local_player_indices[local_player_index];
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    player->unit_index = new_unit_index;
    player->dead_unit_index = -1;
    player_control_new_unit(local_player_index, new_unit_index);
}
