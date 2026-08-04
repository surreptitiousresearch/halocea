/* equipment_build_update_delta @0x837B6330 — encode a network update-delta for an equipment object.
 * Sibling of weapon_build_update_delta.c/projectile_build_update_delta.c: message header is (translated
 * object index, two raw per-object bytes at +581/+582 — the second a rolling per-object update sequence
 * number incremented mod 255 on a successful encode — and whether `mode` is stateless); payload for the
 * incremental case is 9 raw dwords (+92/+96/+100/+104/+108/+112, then +140/+144/+148); buffer for the
 * stateless/baseline case starts at +584. Dispatches to the incremental or stateless message_delta_processor
 * encoder depending on `mode`. Returns the encoded size in bits, or 0 if the object handle is stale/
 * wrong-typed. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/message_delta_processor_mode.h"
#include "headers/message_delta_message_ids.h"
#include "headers/equipment_datum.h"
#include "headers/equipment_update_header.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern message_delta_processor_message_definition_type object_type_get_update_message_type(int16_t object_type);
extern int message_delta_processor_encode_incremental(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, const void *baseline_data, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);

int equipment_build_update_delta(int object_index, void *buffer, int buffer_size_in_bits,
    const message_delta_processor_mode mode)
{
    equipment_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_equipment);
    if ( !object )
        return 0;

    int translated_index = field_translated_index_translate_index(&field_properties_object_index_definition, object_index);
    __int16 object_type = object->object.type;

    _equipment_update_header header;
    header.translated_object_index = translated_index;
    header.baseline_index = object->equipment.baseline_index;
    header.sequence_number = object->equipment.message_index;
    header.mode_is_stateless = (mode == _message_delta_mode_stateless);

    message_delta_processor_message_definition_type update_message_type = object_type_get_update_message_type(object_type);

    int result;
    if ( mode == _message_delta_mode_incremental )
    {
        /* motion state bit-copied (float->int) into the encoder's 9-dword source blob */
        int source_data[9];
        source_data[0] = *(int *)&object->object.position.n[0];
        source_data[1] = *(int *)&object->object.position.n[1];
        source_data[2] = *(int *)&object->object.position.n[2];
        source_data[3] = *(int *)&object->object.translational_velocity.n[0];
        source_data[4] = *(int *)&object->object.translational_velocity.n[1];
        source_data[5] = *(int *)&object->object.translational_velocity.n[2];
        source_data[6] = *(int *)&object->object.angular_velocity.n[0];
        source_data[7] = *(int *)&object->object.angular_velocity.n[1];
        source_data[8] = *(int *)&object->object.angular_velocity.n[2];

        result = message_delta_processor_encode_incremental(update_message_type, &header, source_data,
            &object->equipment.baseline, buffer, buffer_size_in_bits, 0);
    }
    else
    {
        result = message_delta_processor_encode_stateless(update_message_type, &header, &object->equipment.baseline,
            buffer, buffer_size_in_bits);
    }

    if ( result > 0 )
    {
        unsigned int next_sequence_number = (unsigned char)(object->equipment.message_index + 1);
        object->equipment.message_index = (unsigned char)next_sequence_number;
        if ( next_sequence_number >= 0xFF )
            object->equipment.message_index = 0;
    }

    return result;
}
