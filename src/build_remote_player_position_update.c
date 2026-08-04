/* build_remote_player_position_update @0x8375722C — on the server, encode an outgoing position update for
 * one remote player. A full baseline (stateless) update is sent reliably whenever the baseline interval
 * has elapsed (or none was ever sent), stashing the new position as the delta baseline; otherwise, once
 * the regular update interval has elapsed, an incremental (delta-from-baseline) update is sent unreliably.
 * Returns the encoded message size in bits (0 if nothing is sent) and reports reliability via
 * is_reliable_send. */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/simple_circular_queue.h"
#include "headers/field_properties_definition.h"
#include "headers/remote_player_position_update_header.h"
#include "headers/remote_player_position_update_network_data.h"
#include "headers/blam_data_globals.h"

typedef struct _field_properties_definition _field_properties_definition;
extern int  _message_remote_player_position_update;

extern int game_time_get(void);
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern int message_delta_processor_encode_incremental(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, const void *baseline_data, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);

int build_remote_player_position_update(player_datum *source_player, int player_index,
                                        unsigned __int8 *is_reliable_send)
{
    *is_reliable_send = 0;

    remote_player_position_update_header header;
    remote_player_position_update_network_data body;
    header.update_id = source_player->___u26.server_update_data.last_completed_update_id;
    header.player_index = field_translated_index_translate_index(&field_properties_player_index_definition, player_index);
    body.position = source_player->___u26.server_update_data.position_as_of_last_completed_update;
    header.sequence_number = source_player->___u26.server_update_data.next_remote_player_update_sequence_number;

    unsigned int baseline_time = source_player->___u26.server_update_data.time_of_last_remote_player_position_baseline_update;
    if (game_time_get() >= baseline_time + remote_player_position_baseline_update_rate || baseline_time == -1)
    {
        int size = message_delta_processor_encode_stateless(_message_remote_player_position_update,
                                                            &header, &body, g_message_encode_buffer, 32760);
        *is_reliable_send = 1;
        /* stash the position as the delta baseline in the position queue's header fields */
        source_player->___u26.client_update_data.___u0.remote_player.position_queue.queue.max_size = *(int *)&body.position.n[0];
        source_player->___u26.client_update_data.___u0.remote_player.position_queue.queue.user_data_element_size = *(int *)&body.position.n[1];
        source_player->___u26.client_update_data.___u0.remote_player.position_queue.queue.entries =
            (simple_circular_queue_entry *)*(int *)&body.position.n[2];
        source_player->___u26.server_update_data.time_of_last_remote_player_position_baseline_update = game_time_get();
        source_player->___u26.server_update_data.time_of_last_remote_player_position_update = game_time_get();
        source_player->___u26.server_update_data.update_id_for_position_baseline = header.update_id;
        return size;
    }
    else if (game_time_get() < source_player->___u26.server_update_data.time_of_last_remote_player_position_update
                             + remote_player_position_update_rate)
    {
        return 0;
    }
    else
    {
        int size = message_delta_processor_encode_incremental(_message_remote_player_position_update,
                       &header, &body, &source_player->___u26.client_update_data.___u0.remote_player.position_queue,
                       g_message_encode_buffer, 32760, 0);
        *is_reliable_send = 0;
        source_player->___u26.server_update_data.time_of_last_remote_player_position_update = game_time_get();
        return size;
    }
}
