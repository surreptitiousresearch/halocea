/* ctf_engine_replicate_game_mode_state_to_network @0x83806FF0 — (server) encode the CTF game mode state
 * as a message delta (incremental against the baseline when mode is set, resynchronizing the baseline
 * from the live struct afterwards; stateless from the baseline otherwise) and send it — to every loaded
 * machine when machine_index is NONE, else to the one machine. Counterpart of
 * ctf_engine_replicate_game_mode_state_from_network.
 *
 * DEVIATION 1: the compiled baseline resync moves score[0]/score[1] as one 8-byte copy that the decompiler
 * renders as a mispaired QWORD (score[1] + team_with_flag); the disasm moves score[0..1] then
 * team_with_flag — written as explicit element copies, as in the from_network sibling.
 * DEVIATION 2: the decompiler passes an uninitialized stack local as send_message_to_machine's stacked
 * priority arg; disasm @0x838070A4 stores the constant 3 (li r11,3 / stw) — the usual priority-3 pattern. */

#include <stdint.h>
#include "headers/message_delta_message_ids.h"
#include "headers/network_game_server.h"
#include "headers/network_message_type.h"
#include "headers/ctf_globals.h"
#include "headers/blam_data_globals.h"


extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern int message_delta_processor_encode_incremental(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, const void *baseline_data, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);

extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern uint8_t network_game_server_send_message_to_machine(struct network_game_server *server, int machine_index, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void ctf_engine_replicate_game_mode_state_to_network(message_delta_processor_mode mode,
                                                     const int machine_index)
{
    int encoded_size;

    if ( mode )
    {
        encoded_size = message_delta_processor_encode_incremental(_message_ctf_update, 0,
                           &ctf_globals, &ctf_globals_baseline, g_message_encode_buffer, 32760, 0);
        ctf_globals_baseline.score[0] = ctf_globals.score[0];
        ctf_globals_baseline.score[1] = ctf_globals.score[1];
        ctf_globals_baseline.team_with_flag = ctf_globals.team_with_flag;
    }
    else
    {
        encoded_size = message_delta_processor_encode_stateless(_message_ctf_update, 0,
                           &ctf_globals_baseline, g_message_encode_buffer, 32760);
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
