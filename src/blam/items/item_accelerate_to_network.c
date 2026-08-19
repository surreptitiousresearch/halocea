/* item_accelerate_to_network @0x836B1940 — encode and broadcast an _message_item_accelerate message-
 * delta so every loaded machine applies the same impulse to an item object. The object index is
 * translated to its network-stable form, the magnitude and direction are packed into an
 * item_accelerate_network_data payload, and the stateless-encoded message is sent to all loaded
 * machines at priority 3.
 *
 * Signature recovered from disasm: object_index in r3, magnitude in f1 (single), direction in r5. */

#include <stdint.h>
#include "headers/real_vector3d.h"
#include "headers/item_accelerate_network_data.h"
#include "headers/field_properties_definition.h"
#include "headers/message_delta_message_ids.h"
#include "headers/blam_data_globals.h"

typedef struct network_game_server network_game_server;

/* network_game_server_send_message_to_all_loaded_machines type selector: message-delta. */
#define NETWORK_MESSAGE_TYPE_MESSAGE_DELTA 1
#define MESSAGE_ENCODE_BUFFER_BITS         0x7FF8

typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index_no_default(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void item_accelerate_to_network(int object_index, float magnitude, real_vector3d *direction)
{
    item_accelerate_network_data payload;
    payload.object_index = field_translated_index_translate_index_no_default(
        &field_properties_object_index_definition, object_index);
    payload.magnitude = magnitude;
    payload.direction = *direction;

    int size_in_bits = message_delta_processor_encode_stateless(
        _message_item_accelerate, 0, &payload, g_message_encode_buffer, MESSAGE_ENCODE_BUFFER_BITS);

    network_game_server *server = global_network_game_server_get();
    network_game_server_send_message_to_all_loaded_machines(
        server, NETWORK_MESSAGE_TYPE_MESSAGE_DELTA, g_message_encode_buffer, size_in_bits,
        0, 0, 0, 3);
}
