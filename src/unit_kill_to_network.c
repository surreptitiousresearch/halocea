/* unit_kill_to_network @0x836C8800 — replicate a unit death/ping to all loaded machines. Takes the kill
 * record by value (32 bytes in r3..r6), rewrites its unit_index field from a local object index to the
 * network-translated index, encodes a stateless unit-kill message-delta, and (if encoding produced any
 * bits) broadcasts it reliably to every loaded machine. The encode/translate/send primitives are the
 * network-message subsystem boundary (see re-source-boundary-conventions). */

#include <stdint.h>
#include "headers/unit_kill_network_data.h"

#include "headers/field_properties_definition.h"
#include "headers/blam_data_globals.h"

#include "headers/network_game_server.h"
typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

#define _message_unit_kill                 0xC  /* DB enum: message-delta definition type */
#define network_message_type_message_delta 1

void unit_kill_to_network(unit_kill_network_data data)
{
    data.unit_index = field_translated_index_translate_index(&field_properties_object_index_definition,
                                                             data.unit_index);

    int size_in_bits = message_delta_processor_encode_stateless(_message_unit_kill, 0, &data,
                           g_message_encode_buffer, 0x7FF8);
    if ( size_in_bits > 0 )
        network_game_server_send_message_to_all_loaded_machines(global_network_game_server_get(),
            network_message_type_message_delta, g_message_encode_buffer, size_in_bits, 1, 0, 0, 3);
}
