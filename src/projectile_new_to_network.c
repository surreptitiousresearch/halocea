/* projectile_new_to_network @0x8375A5A8 — server-side encoder: snapshots a newly-created projectile
 * object's networked state (definition, owner identity, position/orientation, velocities, baseline)
 * into a projectile_new_data body and message-delta-encodes it into the supplied buffer. The owner
 * object/player indices are run through the field-translated-index tables (registering a fresh server
 * index for this projectile if it doesn't have one yet). Returns the encoded size in bits. */

#include "headers/data_array.h"
#include "headers/projectile_new_data.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_header_datum.h"
#include "headers/projectile_datum.h"

struct field_properties_definition;

#include "headers/field_properties_definition.h"
#include "headers/blam_data_globals.h"
#include "headers/message_delta_message_ids.h"
typedef struct _field_properties_definition _field_properties_definition;

extern int field_translated_index_translate_index_no_default(const _field_properties_definition *const field_properties_definition, int local_index);
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int field_translated_index_server_register_local_index(const _field_properties_definition *field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);

int projectile_new_to_network(int object_index, void *buffer, int buffer_size_in_bits)
{
    projectile_datum *object = (projectile_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    int server_object_index =
        field_translated_index_translate_index_no_default(&field_properties_object_index_definition, object_index);
    int owner_object_index =
        field_translated_index_translate_index(&field_properties_object_index_definition, object->object.owner_object_index);
    int owner_player_index =
        field_translated_index_translate_index(&field_properties_player_index_definition, object->object.owner_player_index);
    if (server_object_index == -1)
        server_object_index =
            field_translated_index_server_register_local_index(&field_properties_object_index_definition, object_index);

    projectile_new_data body;
    body.definition_index = object->definition_index;
    body.server_object_index = server_object_index;
    body.owner_team_index = object->object.owner_team_index;
    body.owner_player_index = owner_player_index;
    body.owner_object_index = owner_object_index;
    body.position = object->projectile.baseline.position;
    body.forward = object->object.forward;
    body.up = object->object.up;
    body.translational_velocity = object->projectile.baseline.translational_velocity;
    body.angular_velocity = object->object.angular_velocity;
    body.current_baseline_index = object->projectile.baseline_index;

    return message_delta_processor_encode_stateless(_message_projectile_new, nullptr, &body, buffer,
                                                    buffer_size_in_bits);
}
