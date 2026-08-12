/* netgame_equipment_new_to_network @0x83746F10 — broadcast a _message_netgame_equipment_new message delta
 * announcing that a netgame-equipment item now exists at server_object_index, carrying item_definition_index
 * and the netgame_equipment placement's index. Reuses the same wire struct as
 * game_engine_update_item_spawn.c's server-side spawn broadcast (there, field 1 holds a baseline id; here it
 * holds item_definition_index — same 10-byte layout, different producer). */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/netgame_equipment_new_message.h"
#include "headers/message_delta_message_ids.h"
#include "headers/blam_data_globals.h"
#include "headers/network_message_type.h"

typedef struct network_game_server network_game_server;


typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index_no_default(const _field_properties_definition *const field_properties_definition, int local_index);
extern int field_translated_index_server_register_local_index(const _field_properties_definition *field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void netgame_equipment_new_to_network(int server_object_index, int item_definition_index, int16_t netgame_equipment_index)
{
    netgame_equipment_new_message message;

    message.server_object_index = field_translated_index_translate_index_no_default(
        &field_properties_object_index_definition, server_object_index);
    if ( message.server_object_index == -1 )
        message.server_object_index = field_translated_index_server_register_local_index(
            &field_properties_object_index_definition, server_object_index);

    message.item_definition_index = item_definition_index;
    message.netgame_equipment_index = netgame_equipment_index;

    int size_in_bits = message_delta_processor_encode_stateless(
        _message_netgame_equipment_new, nullptr, &message, g_message_encode_buffer, 32760);

    network_game_server *server = global_network_game_server_get();
    network_game_server_send_message_to_all_loaded_machines(
        server, network_message_type_message_delta, g_message_encode_buffer, size_in_bits, 1, 0, 0, 3);
}
