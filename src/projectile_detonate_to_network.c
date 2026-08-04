/* projectile_detonate_to_network @0x83759198 — (server) broadcast a projectile detonation: encode the
 * translated object index plus the projectile object's position (+92/+96/+100) as
 * a stateless message delta and send it to every loaded machine, then mark the object's state dword (+4)
 * detonated (3) and, unless its header entry's persistent-object flag (byte +2 bit 0x8) is set, drop its
 * translated-index registration. */

#include <stdint.h>
#include "headers/message_delta_message_ids.h"
#include "headers/field_properties_definition.h"
#include "headers/network_game_server.h"
#include "headers/network_message_type.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"
#include "headers/object_header_flags.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index_no_default(const _field_properties_definition *const field_properties_definition, int local_index);
extern int field_translated_index_unregister_local_index(const _field_properties_definition *field_properties_definition, int local_index);

extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);

extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);

void projectile_detonate_to_network(int object_index)
{
    object_header_datum *header_entry = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    projectile_datum *projectile = (projectile_datum *)header_entry->datum;

    int payload[4];
    payload[0] = field_translated_index_translate_index_no_default(&field_properties_object_index_definition, object_index);
    /* raw float position dwords copied bit-exact into the int payload */
    payload[1] = *(int *)&projectile->object.position.n[0];
    payload[2] = *(int *)&projectile->object.position.n[1];
    payload[3] = *(int *)&projectile->object.position.n[2];

    int encoded_size = message_delta_processor_encode_stateless(_message_projectile_detonate, 0, payload,
                            g_message_encode_buffer, 32760);

    network_game_server *server = global_network_game_server_get();
    network_game_server_send_message_to_all_loaded_machines(server, network_message_type_message_delta,
        g_message_encode_buffer, encoded_size, 1u, 0, 0, 3);

    ((int *)header_entry->datum)[1] = 3;

    if ( (header_entry->flags & (1u << _object_header_being_deleted_bit)) == 0 )
        field_translated_index_unregister_local_index(&field_properties_object_index_definition, object_index);
}
