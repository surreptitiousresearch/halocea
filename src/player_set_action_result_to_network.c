/* player_set_action_result_to_network @0x836A9990 — replicate a player action result (e.g. picked up /
 * swapped a weapon, entered a seat) to all loaded client machines: build the message body with wire-
 * translated player/object/weapon indices, encode it as a stateless "set action result" message-delta,
 * and if encoding produced bits, send it reliably to every machine. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/player_set_action_result_message.h"
#include "headers/message_delta_message_ids.h"
#include "headers/network_message_type.h"
#include "headers/blam_data_globals.h"

typedef struct network_game_server network_game_server;

typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
/* attested: _message_player_set_action_result (10) and network_message_type_message_delta (1) are
 * immediate enum constants in the binary (li r3,0xA / li r4,1), not globals — the old extern int
 * declarations would have compiled to loads. */
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void player_set_action_result_to_network(
    int player_index,
    networked_set_action_result_type action_result_type,
    int action_object_index,
    int16_t action_result,
    int16_t action_seat_index,
    int weapon_to_drop_as_result_of_swap)
{
    player_set_action_result_message message;
    message.player_index = field_translated_index_translate_index(&field_properties_player_index_definition, player_index);
    message.action_result_type = action_result_type;
    message.action_object_index = field_translated_index_translate_index(&field_properties_object_index_definition, action_object_index);
    message.action_result = action_result;
    message.action_seat_index = action_seat_index;
    message.weapon_to_drop_as_result_of_swap = field_translated_index_translate_index(&field_properties_object_index_definition, weapon_to_drop_as_result_of_swap);

    int size_in_bits = message_delta_processor_encode_stateless(
        _message_player_set_action_result, 0, &message, g_message_encode_buffer, 32760);
    if ( size_in_bits > 0 )
    {
        network_game_server *server = global_network_game_server_get();
        network_game_server_send_message_to_all_loaded_machines(
            server, network_message_type_message_delta, g_message_encode_buffer, size_in_bits, 1, 0, 0, 3);
    }
}
