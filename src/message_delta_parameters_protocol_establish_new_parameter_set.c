/* message_delta_parameters_protocol_establish_new_parameter_set @0x838121F8 — begin a parameter-set
 * protocol changeover: snapshot the current parameters, pack them for transmission, and broadcast a
 * stateless "parameters protocol" message-delta carrying the next protocol number (mod 4) to all machines.
 * Only commits the new protocol number if the broadcast succeeds. Guarded by the changeover-enabled flag
 * and brackets the work with the changeover-message flag. */

#include <stdint.h>
#include "headers/packed_parameter.h"
#include "headers/blam_data_globals.h"
#include "headers/message_delta_message_ids.h"
#include "headers/network_message_type.h"

typedef struct network_game_server network_game_server;

extern void dump_to_parameters_set(void);
extern void pack_parameter_set_for_transmission(packed_parameter *params);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_machines(network_game_server *server, network_message_type type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void message_delta_parameters_protocol_establish_new_parameter_set(void)
{
    if ( g_protocol_changeover_enabled == 1 )
    {
        g_protocol_changeover_message_flag = 1;
        int new_protocol_number = (int)(g_parameters_protocol_number + 1) % 4;

        unsigned char message_body[4];
        message_body[0] = new_protocol_number;
        dump_to_parameters_set();

        packed_parameter packed;
        pack_parameter_set_for_transmission(&packed);

        int size_in_bits = message_delta_processor_encode_stateless(
            _message_parameters_protocol, 0, message_body, g_message_encode_buffer, 32760);
        if ( size_in_bits > 0 )
        {
            network_game_server *server = global_network_game_server_get();
            if ( network_game_server_send_message_to_all_machines(
                     server, network_message_type_message_delta, g_message_encode_buffer, size_in_bits, 1, 0, 1, 3) )
            {
                g_parameters_protocol_number = new_protocol_number;
            }
        }
        g_protocol_changeover_message_flag = 0;
    }
}
