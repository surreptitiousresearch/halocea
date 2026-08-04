/* player_update_server_remote_player_action_update_to_network @0x83757970 — server-side: package one remote
 * player's control action into a network update and either queue it into a super (batched) update or encode it
 * as a standalone message-delta. A baseline (stateless) update is emitted when the baseline interval has elapsed
 * (or none has ever been sent); otherwise an incremental update is emitted, but only once the per-update rate
 * limit has passed. Baseline sends also refresh the player's stored baseline block and toggle its baseline id.
 *
 * DEVIATION: Hex-Rays emitted "local variable allocation has failed" — the 32-byte `player_action` is passed by
 * value, split across r6..r9 and the stack, which it rendered as a pile of phantom args (a5..a20). The DB/PDB
 * prototype (4 args, action by value) is authoritative; disasm confirms the homed struct lives at the incoming
 * arg area and the copy loop is `data.action = action`, with the euler source = &action.desired_facing.
 * DEVIATION: the skip branch's `is_baseline` is a dead read of an uninitialized local in the shipped code (it is
 * never used because should_send is 0 there); set to 0 here.
 * DEVIATION: on a successful standalone encode the shipped build fetches the server and walks every player datum
 * with an EMPTY loop body (verified in disasm) — the per-player send is not present in this build; reproduced. */

#include <stdint.h>
#include "headers/remote_player_action_update_header.h"
#include "headers/remote_player_action_update_network_data.h"
#include "headers/player_action.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/data_iterator.h"
#include "headers/super_remote_players_action_update.h"
#include "headers/field_properties_definition.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/blam_data_globals.h"

#include "headers/network_game_server.h"
typedef struct _field_properties_definition _field_properties_definition;
#include "headers/super_remote_players_action_update.h"
extern int _message_remote_player_action_update;

extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int game_time_get(void);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern int message_delta_processor_encode_incremental(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, const void *baseline_data, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);
extern void add_update_to_super_remote_players_action_update(super_remote_players_action_update *update, remote_player_action_update_header *header, remote_player_action_update_network_data *update_data);
extern network_game_server *global_network_game_server_get(void);
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);

void player_update_server_remote_player_action_update_to_network(int player_index, int update_id,
        int ticks_to_apply_update_to, player_action action)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    player_update_server_data *server_data = &player->___u26.server_update_data;

    remote_player_action_update_header header;
    header.update_id = update_id;
    if ( !use_super_remote_players_action_update )
        player_index = field_translated_index_translate_index(&field_properties_player_index_definition, player_index);
    header.player_index = player_index;
    header.baseline_id = server_data->action_baseline_id;

    remote_player_action_update_network_data data;
    data.ticks_to_apply_update_to = ticks_to_apply_update_to;
    data.action = action;
    vector3d_from_euler_angles2d(&data.facing_vector, &action.desired_facing);

    unsigned char should_send = 1;
    unsigned char is_baseline;
    int last_baseline_time = server_data->time_of_last_remote_player_action_baseline_update;
    if ( (unsigned int)game_time_get() >= (unsigned int)(last_baseline_time + remote_player_action_baseline_update_rate)
            || last_baseline_time == -1 )
    {
        is_baseline = 1;
        server_data->remote_player_action_update_baseline = data;   /* refresh stored baseline block */
        server_data->time_of_last_remote_player_action_baseline_update = game_time_get();
        server_data->time_of_last_remote_player_action_update = game_time_get();
        int next_baseline_id = (server_data->action_baseline_id + 1) % 2;
        server_data->action_baseline_id = next_baseline_id;
        server_data->update_id_for_action_baseline = header.update_id;
        header.baseline_id = next_baseline_id;
    }
    else if ( (unsigned int)game_time_get() < (remote_player_action_update_rate + server_data->time_of_last_remote_player_action_update) )
    {
        is_baseline = 0;   /* unused when should_send == 0 (shipped reads an uninit local here) */
        should_send = 0;
    }
    else
    {
        is_baseline = 0;
        server_data->time_of_last_remote_player_action_update = game_time_get();
    }

    if ( use_super_remote_players_action_update )
    {
        if ( should_send == 1 )
        {
            if ( is_baseline == 1 )
                add_update_to_super_remote_players_action_update(
                        &g_stateless_super_remote_players_action_udpate, &header, &data);
            else
                add_update_to_super_remote_players_action_update(
                        &g_incremental_super_remote_players_action_upate, &header, &data);
        }
    }
    else if ( should_send == 1 )
    {
        int encoded_bits;
        if ( is_baseline == 1 )
            encoded_bits = message_delta_processor_encode_stateless(_message_remote_player_action_update,
                    &header, &data, g_message_encode_buffer, 32760);
        else
            encoded_bits = message_delta_processor_encode_incremental(_message_remote_player_action_update,
                    &header, &data, &server_data->remote_player_action_update_baseline, g_message_encode_buffer, 32760, 1);

        if ( encoded_bits > 0 )
        {
            global_network_game_server_get();
            data_iterator iterator;
            data_iterator_new(&iterator, player_data);
            while ( data_iterator_next(&iterator) )
                ;   /* empty broadcast pass (shipped build) */
        }
    }
}
