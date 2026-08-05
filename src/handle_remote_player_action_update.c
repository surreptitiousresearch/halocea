/* handle_remote_player_action_update @0x8376E0B8 — apply an incoming remote-player action update. For a
 * stateless (is_stateless == 1) update, just store it as the new action-update baseline. Otherwise, only
 * accept it if its baseline_id matches the last acked baseline, and only queue the action if its update_id
 * is within a 32-tick window (forward or up to 64 ticks wrapped) of the player's next expected sequence
 * number; a baseline mismatch is logged and dropped. Once accepted, and only if this isn't the player's
 * very first update (next_local_player_update_sequence_number != -1), derives the desired facing from the
 * update's facing vector, clears the desired zoom level, and pushes the action onto the player's replay
 * queue (logging an overflow if the queue is full). The player's next expected sequence number is always
 * advanced to this update's id once accepted.
 *
 * DEVIATION: the decompiler renders the `action_queue_write` call with garbled arguments
 * (`(action_queue *)update_data->action.control_flags`, `(player_action *)LODWORD(...desired_facing.n[1])`,
 * etc.) — the real call passes the action_queue pointer, the entire 32-byte `update_data->action` by value
 * (PPC ABI splits struct-by-value args across GPRs, which the decompiler mis-attributed to unrelated
 * fields; same class as player_update_client_local_player_update_from_network.c), and, per disasm,
 * `player_update_header->update_id` / `update_data->ticks_to_apply_update_to` for the trailing two scalar
 * arguments — not `update_data->action.desired_weapon_index` as the decompiler's flattened view implies. */

#include <stdint.h>
#include "headers/remote_player_action_update_header.h"
#include "headers/remote_player_action_update_network_data.h"
#include "headers/player_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


extern void *memcpy(void *destination, const void *source, unsigned int size);
extern void *datum_try_and_get(const data_array *data, int index);
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);
extern void player_update_client_log_specific_remote_player(player_datum *remote_player, int log_level, const char *format, ...);
extern uint8_t action_queue_write(action_queue *queue, player_action action, int client_update_id, int ticks_to_apply_update_to);
extern int game_time_get(void);

void handle_remote_player_action_update(remote_player_action_update_header *player_update_header, remote_player_action_update_network_data *update_data, uint8_t is_stateless)
{
    uint8_t accept = 0;
    player_datum *player = datum_try_and_get(player_data, player_update_header->player_index);

    if ( player )
    {
        if ( is_stateless == 1 )
        {
            player->___u26.server_update_data.last_acked_update_id = player_update_header->baseline_id;
            memcpy(&player->___u26.client_update_data.___u0.remote_player.action_update_baseline, update_data,
                sizeof(player->___u26.client_update_data.___u0.remote_player.action_update_baseline));
            accept = 1;
        }
        else if ( player_update_header->baseline_id == player->___u26.server_update_data.last_acked_update_id )
        {
            int delta = player_update_header->update_id - player->___u26.server_update_data.next_local_player_update_sequence_number;
            if ( player_update_header->update_id <= player->___u26.server_update_data.next_local_player_update_sequence_number )
                accept = delta + 64 < 32;
            else
                accept = delta < 32;
        }
        else
        {
            player_update_client_log_specific_remote_player(player, 2,
                "[%d]: Threw away remote player action update with base baseline, [%d] != [%d].",
                game_time_get(), player_update_header->baseline_id, player->___u26.server_update_data.last_acked_update_id);
        }
    }

    if ( accept )
    {
        if ( player->___u26.server_update_data.next_local_player_update_sequence_number != -1 )
        {
            euler_angles2d_from_vector3d(&update_data->action.desired_facing, &update_data->facing_vector);
            update_data->action.desired_zoom_level = -1;
            player_update_client_log_specific_remote_player(player, 2, "Received action [%d]",
                player_update_header->update_id);

            if ( !action_queue_write(&player->___u26.client_update_data.___u0.remote_player.action_queue, update_data->action,
                     player_update_header->update_id, update_data->ticks_to_apply_update_to) )
            {
                player_update_client_log_specific_remote_player(player, 2,
                    "[%d]: Remote player action_queue overflow.\n", game_time_get());
            }
        }
        player->___u26.server_update_data.next_local_player_update_sequence_number = player_update_header->update_id;
    }
}
