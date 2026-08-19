/* player_pseudo_kill @0x836A86B0 — kill a player's unit without a real damage event (used for the
 * failed-teleport "someone's in the way" case): records the kill for scoring, runs the normal death path,
 * then hides/deactivates the corpse and its dropped weapon. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/players_global_data.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


extern uint8_t game_engine_can_score(void);
extern void game_statistics_record_kill(int object_index, int owner_player_index, int owner_object_index, int16_t owner_team_index);
extern void player_died(int player_index);
extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern void object_deactivate(int object_index);
extern void object_set_visibility(int object_index, uint8_t visible_flag);

void player_pseudo_kill(int player_index, int source_unit_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    if ( player->unit_index == -1 )
        return;

    if ( game_engine_can_score() )
        game_statistics_record_kill(player->unit_index, -1, -1, -1);

    players_globals->local_player_failed_teleport_unit_indices[player->local_player_index] = player->unit_index;
    player_died(player_index);

    int unit_index = players_globals->local_player_failed_teleport_unit_indices[player->local_player_index];
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    int weapon = unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_index);

    unit->unit.player_index = -1;
    object_deactivate(unit_index);
    object_set_visibility(unit_index, 0);
    if ( weapon != -1 )
        object_set_visibility(weapon, 0);

    if ( source_unit_index != -1 )
        player->dead_unit_index = source_unit_index;

    players_globals->all_dead = 0;
}
