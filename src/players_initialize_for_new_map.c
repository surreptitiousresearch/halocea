/* players_initialize_for_new_map @0x836A7948 — reset the players subsystem for a new map: dispose player
 * control, zero the globals and set the per-local-player / network sentinels to -1, and revalidate the player,
 * team, and machine-to-player tables. */

#include <string.h>
#include "headers/players_global_data.h"
#include "headers/data_array.h"
#include "headers/player_respawn_failure.h"
#include "headers/blam_data_globals.h"


extern void player_control_dispose(void);
extern void data_make_valid(data_array *data);

void players_initialize_for_new_map(void)
{
    player_control_dispose();
    memset(players_globals, 0, sizeof(players_global_data));
    players_globals->local_player_indices[0] = -1;
    players_globals->local_player_indices[1] = -1;
    players_globals->local_player_failed_teleport_unit_indices[0] = -1;
    players_globals->local_player_failed_teleport_unit_indices[1] = -1;
    players_globals->local_player_network_identifier = -1;
    players_globals->input_inhibited = 0;
    players_globals->players_are_double_speed_duration = 0;
    players_globals->all_dead = 0;
    players_globals->bsp_switch_trigger_index = -1;
    players_globals->respawn_failure_reason = _player_respawn_failure_none;
    data_make_valid(player_data);
    data_make_valid(team_data);
    memset(machine_to_player_table, -1, sizeof(machine_to_player_table));
}
