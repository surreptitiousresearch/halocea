/* object_delete_from_network @0x836F1CF0 — (client only) apply an "object delete" message-delta: resolve
 * the network object index to a local object; unless the object header's flag bit 3 (0x8) is set, drop its
 * translated-index registration, then delete the replicated object. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_header_flags.h"
#include "headers/field_properties_definition.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern int field_translated_index_unregister_local_index(const _field_properties_definition *field_properties_definition, int local_index);

extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern void object_delete_replicated_from_server(int object_index);

void object_delete_from_network(message_delta_processor_header *const header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    int translated_index;
    if ( !message_delta_processor_decode_stateless(&translated_index, header) )
        return;

    int object_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
                                                              translated_index);
    if ( object_index != -1 )
    {
        if ( (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->flags & (1u << _object_header_being_deleted_bit)) == 0 )
        {
            field_translated_index_unregister_local_index(&field_properties_object_index_definition,
                                                          object_index);
        }
        object_delete_replicated_from_server(object_index);
    }
}
