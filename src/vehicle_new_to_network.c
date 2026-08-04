/* vehicle_new_to_network @0x8375DBC8 — the vehicle object_new_to_network serializer. Network-translates the
 * vehicle's own / owner object indices and owner player index (registering a fresh server-local index for
 * the object if it has none yet), translates its 4 seat-occupant object indices, copies its identity,
 * ownership, and several opaque vehicle-specific fields into a message body, and stateless-encodes it into
 * the supplied buffer. Sibling of biped_new_to_network.c/weapon_new_to_network_with_flags.c, sharing the
 * same object_datum-relative field offsets for definition_index/owner_team_index/owner_player_index/
 * owner_object_index. Returns the number of bits written. */

#include "headers/field_properties_definition.h"
#include "headers/message_vehicle_new.h"
#include "headers/message_delta_message_ids.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int field_translated_index_translate_index_no_default(const _field_properties_definition *const field_properties_definition, int local_index);
extern int field_translated_index_server_register_local_index(const _field_properties_definition *field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);

int vehicle_new_to_network(int object_index, void *buffer, int buffer_size_in_bits)
{
    object_datum *object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
    vehicle_datum *vehicle = (vehicle_datum *)object;

    int translated_object_index =
        field_translated_index_translate_index_no_default(&field_properties_object_index_definition, object_index);
    int owner_object_index =
        field_translated_index_translate_index(&field_properties_object_index_definition, object->object.owner_object_index);
    int owner_player_index =
        field_translated_index_translate_index(&field_properties_player_index_definition, object->object.owner_player_index);

    if ( translated_object_index == -1 )
        translated_object_index =
            field_translated_index_server_register_local_index(&field_properties_object_index_definition, object_index);

    s_message_vehicle_new message;
    message.definition_index = object->definition_index;
    message.server_object_index = translated_object_index;
    message.owner_team_index = object->object.owner_team_index;
    message.owner_player_index = owner_player_index;
    message.owner_object_index = owner_object_index;

    /* deviation: the compiled loop is a pre-incrementing cursor starting at unit+256, so the four
     * reads are abs 760/764/768/772 = unit.weapon_object_indices[0..3] (a prior pass misread the
     * pre-increment as a raw +756 block; verified against the DB pseudocode @0x8375DC84). */
    for ( int i = 0; i < 4; ++i )
        message.weapon_object_indices[i] =
            field_translated_index_translate_index_no_default(&field_properties_object_index_definition,
                                                              vehicle->unit.weapon_object_indices[i]);

    message.current_baseline_index = vehicle->vehicle.baseline_index;
    message.position = vehicle->vehicle.baseline.position;
    message.translational_velocity = vehicle->vehicle.baseline.translational_velocity;
    message.angular_velocity = vehicle->vehicle.baseline.angular_velocity;
    message.forward = vehicle->vehicle.baseline.forward;
    message.up = vehicle->vehicle.baseline.up;

    return message_delta_processor_encode_stateless(_message_vehicle_new, nullptr, &message, buffer, buffer_size_in_bits);
}
