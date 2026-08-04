/* oddball_engine_replicate_game_mode_state_to_network @0x83816C40 — (server) encode and broadcast the
 * current Oddball game mode state. Stateless mode encodes oddball_globals_baseline directly. Incremental
 * mode works off a scratch copy of oddball_globals_baseline: unless the ball type is "terminator", every
 * team_score/individual_score entry in the scratch copy is rescaled down by 30 (inverse of the *30 rescale
 * applied on decode) before the delta is computed against the baseline, then individual_score/team_score/
 * current_ball_owner are resynced from the scratch copy back into the baseline. On a successful encode,
 * broadcasts to all loaded machines, or to a specific machine if `machine_index != -1`.
 *
 * DEVIATION: the decompiler shows the trailing `priority` argument to
 * `network_game_server_send_message_to_machine` as an uninitialized-looking stack local (`v8`, never
 * assigned in the visible decompile) — disasm confirms it's a literal `3` spilled to the stack (9th arg,
 * beyond the 8-GPR window), matching the literal `3` already used for the all-machines broadcast. */

#include <stdint.h>
#include "headers/message_delta_processor_mode.h"
#include "headers/oddball_globals.h"
#include "headers/game_variant.h"
#include "headers/oddball_ball_type.h"
#include "headers/network_game_server.h"
#include "headers/network_message_type.h"
#include "headers/message_delta_message_ids.h"
#include "headers/blam_data_globals.h"


extern game_variant *game_engine_get_variant(void);
extern void *memcpy(void *destination, const void *source, unsigned int size);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern int message_delta_processor_encode_incremental(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, const void *baseline_data, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern uint8_t network_game_server_send_message_to_machine(struct network_game_server *server, int machine_index, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void oddball_engine_replicate_game_mode_state_to_network(message_delta_processor_mode mode, int machine_index)
{
    int size_in_bits;

    if ( mode )
    {
        oddball_globals_type scratch;
        memcpy(&scratch, &oddball_globals_baseline, sizeof(oddball_globals_baseline));

        if ( game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type != _oddball_terminator )
        {
            for ( int i = 0; i < 16; i++ )
            {
                scratch.team_score[i] /= 30;
                scratch.individual_score[i] /= 30;
            }
        }

        size_in_bits = message_delta_processor_encode_incremental(_message_oddball_update, nullptr, &scratch,
            &oddball_globals_baseline, g_message_encode_buffer, 32760, 0);

        memcpy(oddball_globals_baseline.individual_score, scratch.individual_score,
            sizeof(oddball_globals_baseline.individual_score));
        memcpy(oddball_globals_baseline.team_score, scratch.team_score, sizeof(oddball_globals_baseline.team_score));
        memcpy(oddball_globals_baseline.current_ball_owner, scratch.current_ball_owner,
            sizeof(oddball_globals_baseline.current_ball_owner));
    }
    else
    {
        size_in_bits = message_delta_processor_encode_stateless(_message_oddball_update, nullptr,
            &oddball_globals_baseline, g_message_encode_buffer, 32760);
    }

    if ( size_in_bits > 0 )
    {
        network_game_server *server = global_network_game_server_get();
        if ( machine_index == -1 )
            network_game_server_send_message_to_all_loaded_machines(server, network_message_type_message_delta,
                g_message_encode_buffer, size_in_bits, 1u, 0, 0, 3);
        else
            network_game_server_send_message_to_machine(server, machine_index, network_message_type_message_delta,
                g_message_encode_buffer, size_in_bits, 1u, 0, 0, 3);
    }
}
