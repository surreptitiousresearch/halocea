/* player_new @0x836AA778 — allocate and initialize a player datum (at a given index, or a fresh one),
 * seeding its name/local-player slot from the supplied network_player (or an empty default), resetting all
 * the per-player state to its "no unit / no target / fresh spawn" defaults, and recording the player in the
 * owning machine's player table (up to 2 players per machine). Returns the new player index, or -1 on
 * allocation failure.
 *
 * All field offsets DB-verified against player_datum / player_update_server_data /
 * multiplayer_player_info (types_members). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/network_player.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"
#include "headers/game_team.h"

#include "headers/data_array.h"
extern const wchar_t empty_wide_string[]; /* .rdata @0x820309EC - the shared L"" literal (def: src/data/empty_wide_string.c) */

extern int datum_new(data_array *data);
extern int datum_new_at_plain_index_hack_for_player_data(data_array *data, int index);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern void *memset(void *dst, int value, unsigned int size);
extern void *memcpy(void *dst, const void *src, unsigned int size);

int player_new(int machine_index, int player_index, uint16_t local_player_index, network_player *network_player_data)
{
    int new_player_index;
    if (player_index == -1)
        new_player_index = datum_new(player_data);
    else
        new_player_index = datum_new_at_plain_index_hack_for_player_data(player_data, player_index);

    int machine_player_count = 0;
    if (new_player_index != -1)
    {
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, new_player_index);
        player_update_server_data *server = &player->___u26.server_update_data;

        const network_player *source = network_player_data ? network_player_data
                                                           : (const network_player *)empty_wide_string;
        ustrncpy(player->name, source->name, 0xBu);
        player->local_player_index = local_player_index;
        player->name[11] = 0;
        player->unit_index = -1;
        player->dead_unit_index = -1;
        player->squad_index = -1;
        player->multiplayer.speed_multiplier = 1.0f;
        player->cluster_index = -1;
        player->aim_assist_unit_index = -1;
        player->team_index = _game_team_player;

        player->action_result = 0;
        player->action_object_index = -1;

        player->quit_out_of_game_time = -1;
        player->quit_out_of_game = 0;
        player->multiplayer.is_odd_man_out = 0;
        server->next_local_player_update_sequence_number = 0;
        server->last_acked_update_id = -1;
        server->time_of_last_ack = 0;
        server->last_completed_update_id = -1;
        server->last_received_update_id = -1;
        server->is_initial_time_stamp_valid = 0;
        server->initial_time_stamp_ms = 0;
        server->total_ticks = 0;
        server->last_time_stamp_ms = 0;
        server->number_of_consecutive_excess_updates = 0;
        server->latched_control_flags = 0;
        server->time_of_last_remote_player_action_update = -1;
        server->time_of_last_remote_player_action_baseline_update = -1;
        server->update_id_for_action_baseline = -1;
        server->action_baseline_id = -1;
        memset(&server->remote_player_action_update_baseline, 0, 0x30u);
        server->next_remote_player_update_sequence_number = 0;
        server->time_of_last_remote_player_position_update = -1;
        server->time_of_last_remote_player_position_baseline_update = -1;
        server->update_id_for_position_baseline = -1;
        server->remote_player_position_update_baseline.position.n[0] = 0.0f;
        server->remote_player_position_update_baseline.position.n[1] = 0.0f;
        server->remote_player_position_update_baseline.position.n[2] = 0.0f;
        server->time_of_last_remote_player_vehicle_update = -1;
        server->time_of_last_remote_player_vehicle_baseline_update = -1;
        server->update_id_for_vehicle_baseline = -1;
        memset(&server->remote_player_vehicle_update_baseline, 0, 0x40u);
        if (network_player_data)
            memcpy(&player->network_player_data, network_player_data, 0x20u);
    }

    /* (8*i) & 0x7FFF8 == 8*(i & 0xFFFF) — 2 int slots per 16-bit machine index */
    for (int *slot = &machine_to_player_table[2 * (machine_index & 0xFFFF)];
         *slot != -1; ++slot)
    {
        if (++machine_player_count >= 2)
            return new_player_index;
    }
    machine_to_player_table[2 * (machine_index & 0xFFFF) + machine_player_count] =
        new_player_index;
    return new_player_index;
}
