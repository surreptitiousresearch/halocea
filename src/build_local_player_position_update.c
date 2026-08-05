/* build_local_player_position_update @0x83756F54 — builds the message-delta body that tells the server
 * this local player's latest completed-update id and position, throttled by local_player_update_rate.
 * Returns the encoded size in bits (0 if there is nothing valid to send or the rate limit blocks it).
 * Advances the local-player update sequence number and records the ack bookkeeping.
 *
 * The reliable-send flag output is always cleared (these updates are sent unreliably). */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/local_player_update_network_data.h"
#include "headers/blam_data_globals.h"

extern uint8_t player_update_history_is_valid_id(int update_id);
extern int game_time_get(void);
extern int  _message_local_player_update;
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern unsigned int GetTickCount(void);
extern void player_update_server_log(const char *format, ...);

int build_local_player_position_update(player_datum *source_player, int player_index,
                                       uint8_t *is_reliable_send)
{
    *is_reliable_send = 0;

    if (player_update_history_is_valid_id(source_player->___u26.server_update_data.last_completed_update_id) != 1)
        return 0;

    local_player_update_network_data body;
    body.position = source_player->___u26.server_update_data.position_as_of_last_completed_update;
    body.sequence_number =
        (uint8_t)source_player->___u26.server_update_data.next_local_player_update_sequence_number;
    body.last_completed_update_id =
        (uint8_t)source_player->___u26.server_update_data.last_completed_update_id;

    if (source_player->___u26.server_update_data.last_acked_update_id != -1
        && (unsigned int)game_time_get() < source_player->___u26.server_update_data.time_of_last_ack
                                           + local_player_update_rate)
    {
        return 0;
    }

    int size_in_bits = message_delta_processor_encode_stateless(
        _message_local_player_update, nullptr, &body, g_message_encode_buffer, 32760);
    *is_reliable_send = 0;

    int last_completed = source_player->___u26.server_update_data.last_completed_update_id;
    source_player->___u26.server_update_data.next_local_player_update_sequence_number =
        (source_player->___u26.server_update_data.next_local_player_update_sequence_number + 1) % 32;

    int now = game_time_get();
    unsigned int tick = GetTickCount();
    player_update_server_log("[%d]: [%d]:\t Acked [%d]\n", tick, now, last_completed);

    source_player->___u26.server_update_data.last_acked_update_id =
        source_player->___u26.server_update_data.last_completed_update_id;
    source_player->___u26.server_update_data.time_of_last_ack = game_time_get();
    return size_in_bits;
}
