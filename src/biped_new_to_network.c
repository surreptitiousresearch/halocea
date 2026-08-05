/* biped_new_to_network @0x837AD820 — the biped object_new_to_network serializer. Network-translates the
 * biped's own / owner object indices and owner player index (registering a fresh server-local index for the
 * object if it has none yet), copies the biped's placement and replicated state into a message body, and
 * stateless-encodes it into the supplied buffer. Returns the number of bits written. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/message_biped_new.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/blam_data_globals.h"

typedef struct _field_properties_definition _field_properties_definition;
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern int field_translated_index_translate_index_no_default(const _field_properties_definition *const field_properties_definition, int local_index);
extern int field_translated_index_server_register_local_index(const _field_properties_definition *field_properties_definition, int local_index);
extern int _message_biped_new;
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);

int biped_new_to_network(int object_index, void *buffer, int buffer_size_in_bits)
{
    biped_datum *object = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);

    int translated_object_index =
        field_translated_index_translate_index_no_default(&field_properties_object_index_definition, object_index);
    int owner_object_index =
        field_translated_index_translate_index(&field_properties_object_index_definition, object->object.owner_object_index);
    int owner_player_index =
        field_translated_index_translate_index(&field_properties_player_index_definition, object->object.owner_player_index);
    if (translated_object_index == -1)
        translated_object_index =
            field_translated_index_server_register_local_index(&field_properties_object_index_definition, object_index);

    s_message_biped_new message;
    message.definition_index = object->definition_index;
    message.server_object_index = translated_object_index;
    message.owner_team_index = object->object.owner_team_index;
    message.owner_player_index = owner_player_index;
    message.owner_object_index = owner_object_index;
    message.position = object->object.position;
    message.forward = object->object.forward;
    message.up = object->object.up;
    message.translational_velocity = object->object.translational_velocity;
    message.change_colors[0] = object->object.base_change_colors[0];
    message.change_colors[1] = object->object.base_change_colors[1];
    message.change_colors[2] = object->object.base_change_colors[2];
    message.change_colors[3] = object->object.base_change_colors[3];
    message.current_baseline_index = object->biped.baseline_index;
    *(int16_t *)message.grenade_counts = *(int16_t *)object->biped.baseline.grenade_counts;
    message.body_vitality = object->biped.baseline.body_vitality;
    message.shield_vitality = object->biped.baseline.shield_vitality;
    message.shield_stun_ticks_greater_than_zero = object->biped.baseline.shield_stun_ticks_greater_than_zero;

    return message_delta_processor_encode_stateless(_message_biped_new, 0, &message, buffer, buffer_size_in_bits);
}
