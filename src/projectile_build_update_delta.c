/* projectile_build_update_delta @0x8375A9B0 — encode a network update-delta for a projectile object:
 * builds a small message header (translated object index, two raw per-object bytes at +634/+635 — the
 * second is a rolling per-object update sequence number, incremented mod 255 on a successful encode — and
 * whether `mode` is stateless) plus the payload (raw projectile dwords at +92/+96/+100, extended with
 * +104/+108/+112 for the incremental case), then dispatches to the incremental or stateless
 * message_delta_processor encoder depending on `mode`. Returns the encoded size in bits, or 0 if the
 * object handle is stale/wrong-typed. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/message_delta_processor_mode.h"
#include "headers/message_delta_message_ids.h"
#include "headers/projectile_datum.h"
#include "headers/projectile_update_header.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern message_delta_processor_message_definition_type object_type_get_update_message_type(int16_t object_type);
extern int message_delta_processor_encode_incremental(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, const void *baseline_data, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);

int projectile_build_update_delta(int object_index, void *buffer, int buffer_size_in_bits,
    const message_delta_processor_mode mode)
{
    projectile_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_projectile);
    if ( !object )
        return 0;

    int translated_index = field_translated_index_translate_index(&field_properties_object_index_definition, object_index);
    __int16 object_type = object->object.type;

    _projectile_update_header header;
    header.translated_object_index = translated_index;
    header.baseline_index = object->projectile.baseline_index;
    header.sequence_number = object->projectile.message_index;
    header.mode_is_stateless = (mode == _message_delta_mode_stateless);

    message_delta_processor_message_definition_type update_message_type = object_type_get_update_message_type(object_type);

    int result;
    if ( mode == _message_delta_mode_incremental )
    {
        /* position + translational_velocity as raw float bits */
        int source_data[6];
        source_data[0] = *(int *)&object->object.position.n[0];
        source_data[1] = *(int *)&object->object.position.n[1];
        source_data[2] = *(int *)&object->object.position.n[2];
        source_data[3] = *(int *)&object->object.translational_velocity.n[0];
        source_data[4] = *(int *)&object->object.translational_velocity.n[1];
        source_data[5] = *(int *)&object->object.translational_velocity.n[2];

        result = message_delta_processor_encode_incremental(update_message_type, &header, source_data,
            &object->projectile.baseline, buffer, buffer_size_in_bits, 0);
    }
    else
    {
        result = message_delta_processor_encode_stateless(update_message_type, &header, &object->projectile.baseline,
            buffer, buffer_size_in_bits);
    }

    if ( result > 0 )
    {
        unsigned int next_sequence_number = (unsigned char)(object->projectile.message_index + 1);
        object->projectile.message_index = (unsigned __int8)next_sequence_number;
        if ( next_sequence_number >= 0xFF )
            object->projectile.message_index = 0;
    }

    return result;
}
