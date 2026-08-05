#include <stdint.h>
#include "headers/blam_data_globals.h"
/* projectile_attach_to_network @0x83759858 — server-side encoder: notifies all loaded machines that a
 * projectile has attached to a parent object at a given node, message-delta-encoding the (network-index-
 * translated) projectile and parent indices plus the parent node index. Mirrors object_delete_to_network.c's
 * encode-then-broadcast pattern. */

typedef struct network_game_server network_game_server;
typedef struct _field_properties_definition _field_properties_definition;
typedef int network_message_type;
typedef int message_delta_processor_message_definition_type;

extern message_delta_processor_message_definition_type _message_projectile_attach;
extern const network_message_type network_message_type_message_delta;

extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void projectile_attach_to_network(int projectile_index, int parent_index, int16_t parent_node_index)
{
    struct
    {
        int     projectile_network_index;
        int     parent_network_index;
        int16_t parent_node_index;
    } body;

    body.projectile_network_index = field_translated_index_translate_index(&field_properties_object_index_definition, projectile_index);
    body.parent_network_index = field_translated_index_translate_index(&field_properties_object_index_definition, parent_index);
    body.parent_node_index = parent_node_index;

    int size_in_bits = message_delta_processor_encode_stateless(_message_projectile_attach, (void *)0, &body,
        g_message_encode_buffer, 32760);

    network_game_server *server = global_network_game_server_get();
    network_game_server_send_message_to_all_loaded_machines(
        server, network_message_type_message_delta, g_message_encode_buffer, size_in_bits, 1u, 0, 0, 3);
}
