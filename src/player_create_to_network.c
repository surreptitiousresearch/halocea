/* player_create_to_network @0x836A92F0 — (server only) encode a "player create" message-delta carrying the
 * player-list slot, the raw player index, its network-translated form, and the team, then send it either to
 * one machine or to all loaded machines (machine_index == -1).
 *
 * DEVIATION: the decompiler rendered the to-machine call's stacked 8th argument (priority) as an
 * uninitialized local; disasm (0x836A9384: li r11,3 / stw r11,var_3C) shows priority 3 is stored to the
 * stack argument slot explicitly, exactly like the all-machines path. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/field_properties_definition.h"
#include "headers/message_definition.h"
#include "headers/message_delta_message_ids.h"
#include "headers/network_game_server.h"
#include "headers/network_message_type.h"
#include "headers/player_create_message.h"
#include "headers/blam_data_globals.h"

typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);

extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);

extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern uint8_t network_game_server_send_message_to_machine(struct network_game_server *server, int machine_index, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void player_create_to_network(uint8_t player_list_index, int player_index, int team_index, int machine_index)
{
    player_create_message message;
    message.player_list_index = player_list_index;
    message.raw_player_index = player_index; /* DB field name is raw_player_index */
    message.translated_player_index =
        field_translated_index_translate_index(&field_properties_player_index_definition, player_index);
    message.team_index = team_index;

    int encoded_size_in_bits = message_delta_processor_encode_stateless(_message_player_create, 0, &message,
                                                                        g_message_encode_buffer, 32760);
    if ( encoded_size_in_bits > 0 )
    {
        network_game_server *server = global_network_game_server_get();
        if ( machine_index == -1 )
            network_game_server_send_message_to_all_loaded_machines(server, network_message_type_message_delta,
                                                                    g_message_encode_buffer,
                                                                    encoded_size_in_bits, 1u, 0, 0, 3);
        else
            network_game_server_send_message_to_machine(server, machine_index,
                                                        network_message_type_message_delta,
                                                        g_message_encode_buffer, encoded_size_in_bits,
                                                        1u, 0, 0, 3);
    }
}
