/* king_engine_replicate_game_mode_state_to_network @0x8382D058 — (server) encode the King of the Hill
 * game mode state as a message delta. Unlike the race/ctf/slayer siblings, the incremental path first
 * copies the whole live king_globals into a scratch buffer and divides every score[] entry by 30 in place
 * (undoing the *30 "display" rescale the from_network path applies on decode) before encoding that scaled
 * copy as source_data against king_globals_baseline; after encoding, the baseline's score[16] and hill_id
 * are resynced from the same scaled scratch copy (not from live king_globals) — so the baseline always
 * holds the raw, unscaled score representation used on the wire. Stateless encodes go straight from the
 * baseline, matching the other siblings. Sends to every loaded machine when machine_index is NONE, else to
 * the one machine. Counterpart of king_engine_replicate_game_mode_state_from_network.
 *
 * DEVIATION: the decompiler passes an uninitialized stack local as send_message_to_machine's stacked
 * priority arg; matches the established pattern (ctf/race_engine_replicate_game_mode_state_to_network.c)
 * of the constant 3. */

#include <stdint.h>
#include <string.h>
#include "headers/message_delta_message_ids.h"
#include "headers/network_game_server.h"
#include "headers/network_message_type.h"
#include "headers/king_globals.h"
#include "headers/message_delta_processor_mode.h"
#include "headers/blam_data_globals.h"


extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern int message_delta_processor_encode_incremental(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, const void *baseline_data, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);

extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern uint8_t network_game_server_send_message_to_machine(struct network_game_server *server, int machine_index, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void king_engine_replicate_game_mode_state_to_network(message_delta_processor_mode mode, int machine_index)
{
    int encoded_size;

    if ( mode )
    {
        king_globals_type scaled;
        memcpy(&scaled, &king_globals, sizeof(king_globals));
        for ( int i = 0; i < 16; i++ )
            scaled.score[i] /= 30;

        encoded_size = message_delta_processor_encode_incremental(_message_king_update, 0,
                           &scaled, &king_globals_baseline, g_message_encode_buffer, 32760, 0);

        memcpy(king_globals_baseline.score, scaled.score, sizeof(king_globals_baseline.score));
        king_globals_baseline.hill_id = scaled.hill_id;
    }
    else
    {
        encoded_size = message_delta_processor_encode_stateless(_message_king_update, 0,
                           &king_globals_baseline, g_message_encode_buffer, 32760);
    }

    if ( encoded_size > 0 )
    {
        network_game_server *server = global_network_game_server_get();
        if ( machine_index == -1 )
            network_game_server_send_message_to_all_loaded_machines(server,
                network_message_type_message_delta, g_message_encode_buffer, encoded_size, 1u, 0, 0, 3);
        else
            network_game_server_send_message_to_machine(server, machine_index,
                network_message_type_message_delta, g_message_encode_buffer, encoded_size, 1u, 0, 0, 3);
    }
}
