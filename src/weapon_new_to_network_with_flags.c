/* weapon_new_to_network_with_flags @0x836DA598 — the weapon object_new_to_network serializer.
 * Network-translates the weapon's own / owner object indices and owner player index (registering a
 * fresh server-local index for the object if it has none yet), copies its identity, ownership,
 * orientation and several opaque weapon-specific fields into a message body, and stateless-encodes it
 * into the supplied buffer. Sibling of biped_new_to_network.c/equipment_new_to_network_with_flags.c,
 * sharing the same object_datum-relative field offsets for
 * definition_index/owner_team_index/owner_player_index/owner_object_index/forward/up. Returns the number
 * of bits written. */

#include "headers/field_properties_definition.h"
#include "headers/message_weapon_new.h"
#include "headers/message_delta_message_ids.h"
#include "headers/data_array.h"
#include "headers/object_datum.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int field_translated_index_translate_index_no_default(const _field_properties_definition *const field_properties_definition, int local_index);
extern int field_translated_index_server_register_local_index(const _field_properties_definition *field_properties_definition, int local_index);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);

int weapon_new_to_network_with_flags(int object_index, void *buffer, int buffer_size_in_bits, int object_flags)
{
    weapon_datum *object =
        (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    int translated_object_index =
        field_translated_index_translate_index_no_default(&field_properties_object_index_definition, object_index);
    int owner_object_index =
        field_translated_index_translate_index(&field_properties_object_index_definition, object->object.owner_object_index);
    int owner_player_index =
        field_translated_index_translate_index(&field_properties_player_index_definition, object->object.owner_player_index);

    if ( translated_object_index == -1 )
        translated_object_index =
            field_translated_index_server_register_local_index(&field_properties_object_index_definition, object_index);

    s_message_weapon_new message;
    message.definition_index = object->definition_index;
    message.server_object_index = translated_object_index;
    message.owner_team_index = object->object.owner_team_index;
    message.owner_player_index = owner_player_index;
    message.owner_object_index = owner_object_index;
    message.object_flags = object_flags;
    message.position = object->weapon.baseline.position;
    message.forward = object->object.forward;
    message.up = object->object.up;
    message.translational_velocity = object->weapon.baseline.translational_velocity;
    message.current_baseline_index = object->weapon.baseline_index;
    message.magazine_0_rounds_total = object->weapon.baseline.magazine_0_rounds_total;
    message.magazine_1_rounds_total = object->weapon.baseline.magazine_1_rounds_total;
    message.age = object->weapon.baseline.age;
    message.magazine_0_rounds_loaded = object->weapon.magazines[0].rounds_loaded;
    message.magazine_1_rounds_loaded = object->weapon.magazines[1].rounds_loaded;

    return message_delta_processor_encode_stateless(_message_weapon_new, nullptr, &message, buffer, buffer_size_in_bits);
}
