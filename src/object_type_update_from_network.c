/* object_type_update_from_network @0x83702780 */
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/field_properties_definition.h"
#include "headers/blam_data_globals.h"


extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern void object_type_process_update_delta(int object_index, message_delta_processor_header *header, network_game_client *client);

void object_type_update_from_network(message_delta_processor_header *header, network_game_client *client)
{
    int *custom_header = (int *)header->custom_header;
    int local_index = field_translated_index_get_local_index(&field_properties_object_index_definition, *custom_header);

    *custom_header = local_index;
    object_type_process_update_delta(local_index, header, client);
}
