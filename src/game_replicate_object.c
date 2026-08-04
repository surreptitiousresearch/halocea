/* game_replicate_object @0x83687F50 — (server) encode and broadcast one object's replication update: for
 * an incremental update, if the object type demands a fresh baseline, updates it and downgrades this send
 * to stateless; otherwise builds the update via object_type_build_update_delta and sends it (to all
 * loaded machines, or one specific machine) as a reliable message-delta. Returns whether it was sent. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_server.h"
#include "headers/network_message_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t object_type_should_force_baseline_update(int object_index);
extern void object_type_update_baseline(int object_index);
extern int object_type_build_update_delta(int object_index, void *buffer, int buffer_size_in_bits, const message_delta_processor_mode mode);

extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern uint8_t network_game_server_send_message_to_machine(struct network_game_server *server, int machine_index, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

int game_replicate_object(int object_index, message_delta_processor_mode mode, int machine_index)
{
    message_delta_processor_mode effective_mode = mode;

    if ( mode == _message_delta_mode_incremental )
    {
        if ( object_type_should_force_baseline_update(object_index) )
        {
            object_type_update_baseline(object_index);
            effective_mode = _message_delta_mode_stateless;
        }
    }

    int encoded_size = object_type_build_update_delta(object_index, g_message_encode_buffer, 32760, effective_mode);
    if ( encoded_size <= 0 )
        return 0;

    unsigned __int8 reliable = effective_mode == _message_delta_mode_stateless;
    network_game_server *server = global_network_game_server_get();

    if ( machine_index == -1 )
        return network_game_server_send_message_to_all_loaded_machines(server, network_message_type_message_delta,
                   g_message_encode_buffer, encoded_size, reliable, 0, 0, 3) == 1;

    return network_game_server_send_message_to_machine(server, machine_index, network_message_type_message_delta,
               g_message_encode_buffer, encoded_size, reliable, 0, 1u, 3) == 1;
}
