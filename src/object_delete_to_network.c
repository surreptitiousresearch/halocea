/* object_delete_to_network @ 0x836EBCF8 — broadcast an object-deletion message from the server so all
 * clients drop the object too. Translates the local object index into the network index space, encodes a
 * stateless "object deletion" message-delta, unregisters the local-index mapping unless the object is
 * already flagged pending-delete (header flag 0x8), and — if encoding produced bits — sends it reliably
 * to every loaded machine. Network types are opaque boundaries; the encode buffer/definitions are
 * engine globals. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_header_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/message_delta_message_ids.h"
#include "headers/network_message_type.h"

typedef struct network_game_server network_game_server;
typedef struct _field_properties_definition _field_properties_definition;
typedef int network_message_type;
typedef int message_delta_processor_message_definition_type;


extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int field_translated_index_unregister_local_index(const _field_properties_definition *field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void object_delete_to_network(int object_index)
{
    int network_index = field_translated_index_translate_index(&field_properties_object_index_definition, object_index);
    int size_in_bits = message_delta_processor_encode_stateless(_message_object_deletion, (void *)0,
                           &network_index, g_message_encode_buffer, 32760);

    object_header_datum *object_header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    if ( (object_header->flags & (1u << _object_header_being_deleted_bit)) == 0 )
        field_translated_index_unregister_local_index(&field_properties_object_index_definition, object_index);

    if ( size_in_bits > 0 )
    {
        network_game_server *server = global_network_game_server_get();
        network_game_server_send_message_to_all_loaded_machines(
            server, network_message_type_message_delta, g_message_encode_buffer, size_in_bits, 1u, 0, 0, 3);
    }
}
