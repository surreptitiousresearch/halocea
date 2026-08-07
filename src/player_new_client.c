/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* player_new_client @0x836AA938 — allocate (or reuse, if player_index != -1) a player_datum slot for a
 * newly-connecting client and initialize it to defaults: name (from network_player_data, or an empty
 * fallback string), local_player_index, squad/team/unit/dead_unit/aim_assist indices, cluster_index,
 * speed_multiplier, action_result/action_object_index, quit_out_of_game_time/quit_out_of_game, and the
 * update-data union's leading dwords. If this is a network (non-local, local_player_index == -1) player,
 * also resets its full client_remote_player_data reception state (action/position/vehicle queues, their
 * baselines, and last-update-id bookkeeping). Copies the raw network_player_data into the player's own
 * network_player_data block if one was supplied. Finally registers the new player index in the connecting
 * machine's player list (max 2 players per machine) and returns the player index (or -1 on allocation
 * failure).
 *
 * Field offsets were resolved against headers/player_datum.h and headers/client_remote_player_data.h;
 * every raw offset in the original decompile matched an already-established field exactly, including the
 * `(position_queue*)v12 + 16` / `(vehicle_update_queue*)v12 + 20` pointer-arithmetic-in-elements forms
 * (confirmed via disasm to be byte offsets 0x180/0x1E0 — i.e. those two queue types are each exactly
 * 24 bytes, matching their declared sizes). */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/multiplayer_team.h"
#include "headers/blam_data_globals.h"

extern const wchar_t empty_wide_string[]; /* .rdata @0x820309EC - the shared L"" literal (def: src/data/empty_wide_string.c) */

extern int datum_new(data_array *data);
extern int datum_new_at_index(data_array *data, int index);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern void action_queue_initialize(action_queue *queue);
extern void position_queue_initialize(position_queue *queue);
extern void vehicle_update_queue_initialize(vehicle_update_queue *queue);
/* memcpy declared by <string.h> */
extern int *machine_get_player_list(int machine_index);

int player_new_client(int machine_index, int player_index, int16_t local_player_index, network_player *network_player_data)
{
    int new_player_index = (player_index == -1) ? datum_new(player_data) : datum_new_at_index(player_data, player_index);
    int machine_slot = 0;

    if ( new_player_index != -1 )
    {
        player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, new_player_index);
        const network_player *name_source = network_player_data ? network_player_data
                : (const network_player *)empty_wide_string;

        ustrncpy(player->name, name_source->name, 0xB);
        player->name[11] = 0;

        player->local_player_index = local_player_index;
        player->squad_index = -1;
        player->team_index = _multiplayer_team_blue;
        player->action_object_index = -1;
        player->action_result = 0;
        player->unit_index = -1;
        player->dead_unit_index = -1;
        player->cluster_index = -1;
        player->aim_assist_unit_index = -1;
        player->multiplayer.speed_multiplier = 1.0f;
        player->quit_out_of_game_time = -1;
        player->quit_out_of_game = 0;
        player->___u26.server_update_data.next_local_player_update_sequence_number = -1;
        player->___u26.server_update_data.last_acked_update_id = -1;

        if ( local_player_index == -1 )
        {
            client_remote_player_data *remote = &player->___u26.client_update_data.___u0.remote_player;

            remote->last_position_update_sequence_number = -1;
            action_queue_initialize(&remote->action_queue);
            remote->last_position_update_id = -1;
            memset(&remote->action_update_baseline, 0, sizeof(remote->action_update_baseline)); /* struct value; needs & */

            memset(&remote->position_update_baseline, 0, sizeof(remote->position_update_baseline));
            position_queue_initialize(&remote->position_queue);
            remote->number_of_outdated_position_updates_ignored_in_a_row = 0;

            remote->last_vehicle_update_id = -1;
            memset(&remote->vehicle_update_baseline, 0, sizeof(remote->vehicle_update_baseline));
            vehicle_update_queue_initialize(&remote->vehicle_update_queue);
            remote->number_of_outdated_vehicle_updates_ignored_in_a_row = 0;
        }

        if ( network_player_data )
            memcpy(&player->network_player_data, network_player_data, sizeof(player->network_player_data));
    }

    int *machine_players = machine_get_player_list(machine_index);
    while ( machine_players[machine_slot] != -1 )
    {
        if ( ++machine_slot >= 2 )
            return new_player_index;
    }
    machine_players[machine_slot] = new_player_index;

    return new_player_index;
}
