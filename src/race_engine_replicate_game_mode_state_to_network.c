/* race_engine_replicate_game_mode_state_to_network @0x8382B1E8 — (server) encode the Race game mode state
 * as a message delta (incremental against the baseline when mode is set, resynchronizing the baseline from
 * the live struct afterwards via the same quirky partial copy as the from_network sibling; stateless from
 * the baseline otherwise) and send it — to every loaded machine when machine_index is NONE, else to the
 * one machine. Counterpart of race_engine_replicate_game_mode_state_from_network.
 *
 * SHIPPED QUIRK (same as the from_network sibling): the resync is a leftover 64-byte doubleword-unrolled
 * copy starting at race_globals_type+0x80, followed by explicit memcpy of first_flag/lap_bit_vector and
 * scalar copies of lap_completed_value/rally_flag — team_laps[14..15] and persistent_team_score are never
 * synced to the baseline by this path either. Reproduced verbatim via the same resync() helper.
 *
 * DEVIATION: the decompiler passes an uninitialized stack local as send_message_to_machine's stacked
 * priority arg; matches the established pattern (ctf_engine_replicate_game_mode_state_to_network.c) of the
 * constant 3 — used directly here without re-verifying via disasm. */

#include <stdint.h>
#include "headers/message_delta_message_ids.h"
#include "headers/network_game_server.h"
#include "headers/network_message_type.h"
#include "headers/race_globals.h"
#include "headers/message_delta_processor_mode.h"
#include "headers/blam_data_globals.h"


extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern int message_delta_processor_encode_incremental(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, const void *baseline_data, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);
extern void *memcpy(void *destination, const void *source, unsigned int size);

extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern uint8_t network_game_server_send_message_to_machine(struct network_game_server *server, int machine_index, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

static void resync(race_globals_type *destination, const race_globals_type *source)
{
    /* recovered: (char*)dst+0x80 -> &dst->lap_bit_vector[15]; preserved 0x40 byte-span quirk
       spanning lap_bit_vector[15], rally_flag, team_laps[0..13] */
    memcpy(&destination->lap_bit_vector[15], &source->lap_bit_vector[15], 0x40u);
    memcpy(destination->first_flag, source->first_flag, sizeof(destination->first_flag));
    memcpy(destination->lap_bit_vector, source->lap_bit_vector, sizeof(destination->lap_bit_vector));
    destination->lap_completed_value = source->lap_completed_value;
    destination->rally_flag = source->rally_flag;
}

void race_engine_replicate_game_mode_state_to_network(message_delta_processor_mode mode, int machine_index)
{
    int encoded_size;

    if ( mode )
    {
        encoded_size = message_delta_processor_encode_incremental(_message_race_update, 0,
                           &race_globals, &race_globals_baseline, g_message_encode_buffer, 32760, 0);
        resync(&race_globals_baseline, &race_globals);
    }
    else
    {
        encoded_size = message_delta_processor_encode_stateless(_message_race_update, 0,
                           &race_globals_baseline, g_message_encode_buffer, 32760);
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
