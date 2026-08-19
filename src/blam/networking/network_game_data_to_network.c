/* network_game_data_to_network @0x83782B08 — (server) broadcast the game-settings update (message-delta id
 * 32) after establishing a fresh protocol parameter set: statelessly encoded and sent reliably at
 * priority 3 — to ALL machines (including the local client) or to one machine. Returns 1 when the encode
 * produced bits and (for the single-machine path) the send succeeded.
 *
 * DEVIATION: as elsewhere in this family, the to-machine stacked priority was rendered as an uninitialized
 * local; disasm (0x83782B8C: li r11,3 / stw) shows the explicit 3. */

#include <stdint.h>
#include "headers/message_definition.h"
#include "headers/message_delta_message_ids.h"
#include "headers/network_game_server.h"
#include "headers/network_message_type.h"
#include "headers/network_game_data.h"
#include "headers/blam_data_globals.h"


extern void message_delta_parameters_protocol_establish_new_parameter_set(void);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);

extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_machines(network_game_server *server, network_message_type type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern uint8_t network_game_server_send_message_to_machine(struct network_game_server *server, int machine_index, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

uint8_t network_game_data_to_network(network_game_data *game, const int machine_index)
{
    message_delta_parameters_protocol_establish_new_parameter_set();

    int encoded_size_in_bits = message_delta_processor_encode_stateless(_message_game_settings_update, 0,
                                                                        game, g_message_encode_buffer, 32760);
    if ( encoded_size_in_bits <= 0 )
        return 0;

    network_game_server *server = global_network_game_server_get();
    if ( machine_index == -1 )
    {
        network_game_server_send_message_to_all_machines(server, network_message_type_message_delta,
                                                         g_message_encode_buffer, encoded_size_in_bits,
                                                         1u, 0, 1u, 3);
        return 1;
    }
    return network_game_server_send_message_to_machine(server, machine_index,
                                                       network_message_type_message_delta,
                                                       g_message_encode_buffer, encoded_size_in_bits,
                                                       1u, 0, 1u, 3) != 0;
}
