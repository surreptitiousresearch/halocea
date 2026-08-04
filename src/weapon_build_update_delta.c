/* weapon_build_update_delta @0x836DA9E8 — encode a network update-delta for a weapon object. Sibling of
 * projectile_build_update_delta.c: message header is (translated object index, two raw per-object bytes at
 * +737/+738 — the second a rolling per-object update sequence number incremented mod 255 on a successful
 * encode — and whether `mode` is stateless); payload for the incremental case is 6 raw dwords
 * (+92/+96/+100/+104/+108/+112) plus two raw words (+694/+706) plus a raw float (+780), packed
 * contiguously; buffer for the stateless/baseline case starts at +740. Dispatches to the incremental or
 * stateless message_delta_processor encoder depending on `mode`. Returns the encoded size in bits, or 0 if
 * the object handle is stale/wrong-typed.
 *
 * DEVIATION: the decompiler over-sizes the payload local to 9 dwords and then declares 3 more separate
 * trailing locals (word/word/float) right after it on the stack; these are the same one 32-byte contiguous
 * by-ref struct, reproduced here as a single properly-sized type — same "split by-ref struct" pattern as
 * other *_build_update_delta / *_from_network call sites in this codebase. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/message_delta_processor_mode.h"
#include "headers/message_delta_message_ids.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_update_header.h"
#include "headers/weapon_update_payload.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern message_delta_processor_message_definition_type object_type_get_update_message_type(int16_t object_type);
extern int message_delta_processor_encode_incremental(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, const void *baseline_data, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);

int weapon_build_update_delta(int object_index, void *buffer, int buffer_size_in_bits,
    const message_delta_processor_mode mode)
{
    weapon_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_weapon);
    if ( !object )
        return 0;

    int translated_index = field_translated_index_translate_index(&field_properties_object_index_definition, object_index);
    __int16 object_type = object->object.type;

    _weapon_update_header header;
    header.translated_object_index = translated_index;
    header.baseline_index = object->weapon.baseline_index;
    header.sequence_number = object->weapon.message_index;
    header.mode_is_stateless = (mode == _message_delta_mode_stateless);

    message_delta_processor_message_definition_type update_message_type = object_type_get_update_message_type(object_type);

    int result;
    if ( mode == _message_delta_mode_incremental )
    {
        _weapon_update_payload payload;
        payload.position.n[0] = object->object.position.n[0];
        payload.position.n[1] = object->object.position.n[1];
        payload.position.n[2] = object->object.position.n[2];
        payload.translational_velocity.n[0] = object->object.translational_velocity.n[0];
        payload.translational_velocity.n[1] = object->object.translational_velocity.n[1];
        payload.translational_velocity.n[2] = object->object.translational_velocity.n[2];
        payload.magazine_0_rounds_total = object->weapon.magazines[0].rounds_total;
        payload.magazine_1_rounds_total = object->weapon.magazines[1].rounds_total;
        payload.age = object->weapon.baseline.age;

        result = message_delta_processor_encode_incremental(update_message_type, &header, &payload,
            &object->weapon.baseline, buffer, buffer_size_in_bits, 0);
    }
    else
    {
        result = message_delta_processor_encode_stateless(update_message_type, &header,
            &object->weapon.baseline, buffer, buffer_size_in_bits);
    }

    if ( result > 0 )
    {
        unsigned int next_sequence_number = (unsigned char)(object->weapon.message_index + 1);
        object->weapon.message_index = (char)next_sequence_number;
        if ( next_sequence_number >= 0xFF )
            object->weapon.message_index = 0;
    }

    return result;
}
