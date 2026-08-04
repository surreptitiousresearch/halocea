/* biped_build_update_delta @0x837ADCC8 — encode a network update-delta for a biped object: builds a
 * small message header (translated object index, two raw per-object bytes at +1319/+1320 — the latter a
 * rolling per-object update sequence number incremented mod 255 on a successful encode — whether `mode`
 * is stateless, the pre-clear value of the +290 "dirty" flag, and the current system time) plus, for the
 * incremental case, a small payload of a raw word/float pair plus a derived boolean, then dispatches to
 * the incremental or stateless message_delta_processor encoder. Clears +290 and +1140 unconditionally.
 * Returns the encoded size in bits, or 0 if the object handle is stale/wrong-typed. Sibling of
 * projectile_build_update_delta.c/weapon_build_update_delta.c/equipment_build_update_delta.c, which
 * share this same header/payload/encode shape for other object types.
 *
 * DEVIATION: `_cntlzw(mode) & 0x20` is the "mode == 0" idiom, simplified to
 * `mode == _message_delta_mode_stateless`. `(-v13 & ~v13) < 0` (v13 = the raw word at +260) is, by
 * exhaustive small-value check (0,1,-1,2,-2), equivalent to the plain comparison `v13 > 0`. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/message_delta_processor_mode.h"
#include "headers/message_delta_message_ids.h"
#include "headers/biped_datum.h"
#include "headers/biped_update_header.h"
#include "headers/biped_update_body.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


typedef struct _field_properties_definition _field_properties_definition;
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int field_translated_index_translate_index(const _field_properties_definition *const field_properties_definition, int local_index);
extern uint32_t system_milliseconds(void);
extern message_delta_processor_message_definition_type object_type_get_update_message_type(int16_t object_type);
extern int message_delta_processor_encode_incremental(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, const void *baseline_data, void *buffer, int buffer_size_in_bits, uint8_t allow_empty_body);
extern int message_delta_processor_encode_stateless(message_delta_processor_message_definition_type definition_type, const void *source_header, const void *source_data, void *buffer, int buffer_size_in_bits);

int biped_build_update_delta(int object_index, void *buffer, int buffer_size_in_bits,
    const message_delta_processor_mode mode)
{
    biped_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_biped);
    if ( !object )
        return 0;

    _biped_update_header header;
    header.translated_object_index = field_translated_index_translate_index(&field_properties_object_index_definition, object_index);
    header.force_shield_update = object->object.force_shield_update;
    header.sequence_number = object->biped.message_index;
    header.baseline_index = object->biped.baseline_index;
    header.mode_is_stateless = (mode == _message_delta_mode_stateless);
    header.timestamp = system_milliseconds();

    message_delta_processor_message_definition_type update_message_type =
        object_type_get_update_message_type(object->object.type);
    object->object.force_shield_update = 0;

    int result;

    if ( mode == _message_delta_mode_incremental )
    {
        _biped_update_body payload;
        payload.shield_vitality = (header.force_shield_update == 1)
            ? object->object.shield_vitality * 0.33333334f
            : object->biped.baseline.shield_vitality;

        __int16 shield_stun_ticks = object->object.shield_stun_ticks;
        /* unit.grenade_counts is char[2]; copied as a 16-bit value spanning both bytes */
        *(__int16 *)payload.grenade_counts = *(__int16 *)&object->unit.grenade_counts[0];
        payload.body_vitality = object->object.body_vitality;
        payload.shield_stun_ticks_greater_than_zero = (shield_stun_ticks > 0);

        result = message_delta_processor_encode_incremental(update_message_type, &header, &payload,
            &object->biped.baseline, buffer, buffer_size_in_bits, 0);
    }
    else
    {
        result = message_delta_processor_encode_stateless(update_message_type, &header, &object->biped.baseline,
            buffer, buffer_size_in_bits);
    }

    if ( result > 0 )
    {
        unsigned int next_sequence_number = (unsigned char)(object->biped.message_index + 1);
        object->biped.message_index = (char)next_sequence_number;
        if ( next_sequence_number >= 0xFF )
            object->biped.message_index = 0;
    }

    object->unit.force_local_update = 0;

    return result;
}
