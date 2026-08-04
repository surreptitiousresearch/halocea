/* player_update_client_local_player_update_from_network @0x8376DD48 — decode a stateless
 * "local player update ack" message from the server and apply it to this client's own player_datum
 * slot: find the first live local player slot (skips slots with local_player_index == 0xFFFF),
 * verify the ack isn't stale via is_local_player_update_in_order(), then update that slot's client
 * update-reception state (last_action_update_id/action_baseline_id/action_update_baseline) and replay
 * the local player's update history from the new baseline.
 *
 * DEVIATION: the decompiler mis-renders the by-value real_point3d starting_position argument to
 * player_update_history_play (it spans two PPC64 GPRs for the 12-byte struct) as a bogus
 * "*(real_point3d**)+65" dereference plus an unrelated ROL-wrapped 5th pointer argument, and gets the
 * trailing destructive/show_results argument values swapped. Resolved via full disasm trace of the call
 * site: destructive=1, show_results=0, vehicle_starting_info=NULL, history=NULL, starting_position is
 * read directly from the player's own action_update_baseline (the value just written above it). */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/player_update_history.h"
#include "headers/local_player_vehicle_update_network_data.h"
#include "headers/real_point3d.h"
#include "headers/client_log_subject.h"
#include "headers/blam_data_globals.h"


#include "headers/player_update_history.h"
#include "headers/local_player_vehicle_update_network_data.h"
extern int game_time_get(void);
extern void player_update_client_log(client_log_subject subject, int log_level, const char *format, ...);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t is_local_player_update_in_order(int sequence_number, int last_update_sequence_number);
extern network_game_client *global_network_game_client_get(void);
extern void player_update_history_play(player_update_history *history, int unit_index, int last_completed_update_id, real_point3d starting_position, local_player_vehicle_update_network_data *vehicle_starting_info, uint8_t destructive, uint8_t show_results);

/* Wire layout of the decoded stateless ack: sequence number, ack byte, then the 3-dword
 * (12-byte) action baseline payload — matches player_datum's client_update_data.remote_player
 * last_action_update_id/action_baseline_id/action_update_baseline[0..11] fields it's copied into. */
typedef struct local_player_update_ack_network_data
{
    unsigned __int8 sequence_number;
    unsigned __int8 acknowledged_sequence_number;
    int              baseline[3];
} local_player_update_ack_network_data;

void player_update_client_local_player_update_from_network(message_delta_processor_header *header,
    network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    local_player_update_ack_network_data ack;
    if ( message_delta_processor_decode_stateless(&ack, header) != 1 )
        return;

    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    player_datum *player = data_iterator_next(&iterator);
    if ( !player )
        return;

    while ( player->local_player_index == (__int16)0xFFFF )
    {
        player = data_iterator_next(&iterator);
        if ( !player )
            return;
    }

    if ( !is_local_player_update_in_order(ack.sequence_number, player->___u26.client_update_data.___u0.remote_player.last_action_update_id) )
        return;

    player_update_client_log(_client_log_local_player, 1, "[%d]: Received ack for update [%d].\n",
        game_time_get(), ack.acknowledged_sequence_number);

    player->___u26.client_update_data.___u0.remote_player.last_action_update_id = ack.sequence_number;
    player->___u26.client_update_data.___u0.remote_player.action_baseline_id = ack.acknowledged_sequence_number;
    /* 12-byte baseline payload copied raw over the head of action_update_baseline
     * (ticks_to_apply_update_to + action.control_flags + action.desired_facing.n[0]) — word cursor */
    int *action_update_baseline_words =
        (int *)&player->___u26.client_update_data.___u0.remote_player.action_update_baseline;
    action_update_baseline_words[0] = ack.baseline[0];
    action_update_baseline_words[1] = ack.baseline[1];
    action_update_baseline_words[2] = ack.baseline[2];

    int index = iterator.index;
    global_network_game_client_get();

    player_datum *slot = DATA_ARRAY_ELEMENT(player_data, player_datum, index);
    real_point3d starting_position = *(real_point3d *)&slot->___u26.client_update_data.___u0.remote_player.action_update_baseline;

    player_update_history_play(nullptr, slot->unit_index, slot->___u26.client_update_data.___u0.remote_player.action_baseline_id,
        starting_position, nullptr, 1u, 0u);
}
