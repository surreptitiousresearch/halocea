/* game_engine_player_score_encode_and_send_score_update @0x8374A5E0 — encode a player_score_update
 * message-delta (id 21; incremental against the baseline when mode says so, else stateless from the
 * baseline block) and send it reliably at priority 3 to one machine or all loaded machines.
 *
 * DEVIATION: as in player_create_to_network, the decompiler rendered the to-machine call's stacked
 * priority as an uninitialized local; disasm (0x8374A67C: li r11,3 / stw) shows the explicit 3.
 * Note the stateless path encodes from `baseline` (not current_state) — as compiled. */

#include <stdint.h>
#include "headers/message_definition.h"
#include "headers/message_delta_message_ids.h"
#include "headers/network_game_server.h"
#include "headers/network_message_type.h"
#include "headers/player_score_network_data.h"
#include "headers/player_score_update_header.h"
#include "headers/iterated_message.h"
#include "headers/blam_data_globals.h"


extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern int message_delta_processor_encode_incremental(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, const void *baseline_data, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);

extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern uint8_t network_game_server_send_message_to_machine(struct network_game_server *server, int machine_index, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void game_engine_player_score_encode_and_send_score_update(const player_score_network_data *current_state,
        player_score_network_data *baseline, player_score_update_header *header,
        message_delta_processor_mode mode, const int machine_index)
{
    int encoded_size_in_bits;
    if ( mode )
        encoded_size_in_bits = message_delta_processor_encode_incremental(_message_player_score_update,
                                   header, current_state, baseline, g_message_encode_buffer,
                                   32760, 0);
    else
        encoded_size_in_bits = message_delta_processor_encode_stateless(_message_player_score_update,
                                   header, baseline, g_message_encode_buffer, 32760);

    if ( encoded_size_in_bits > 0 )
    {
        network_game_server *server = global_network_game_server_get();
        if ( machine_index == -1 )
            network_game_server_send_message_to_all_loaded_machines(server,
                    network_message_type_message_delta, g_message_encode_buffer, encoded_size_in_bits,
                    1u, 0, 0, 3);
        else
            network_game_server_send_message_to_machine(server, machine_index,
                    network_message_type_message_delta, g_message_encode_buffer, encoded_size_in_bits,
                    1u, 0, 0, 3);
    }
}
