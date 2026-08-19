/* encode_iterated_body_field @0x837A0CF0 — encode one body field of the current message-delta iteration by
 * invoking the field type's encode callback over the field's value (and its baseline, in incremental mode),
 * writing into the iteration data stream. In incremental mode a "changed" bit is also written into the
 * field-map stream and returned; in stateless mode the return is whether any bits were written. The field's
 * bit cost is added to the iteration body total, recorded in last_iteration_field_map, and reported to the
 * metrics layer. */

#include <stdint.h>
#include "headers/iterated_message.h"
#include "headers/message_definition_field_reference_set.h"
#include "headers/blam_data_globals.h"

#include "headers/bitstream_t.h"

extern unsigned int bitstream_write_bit(bitstream_t *bit_stream, const unsigned int value);
extern void metrics_record_field_encode(const _message_definition *message_definition, message_delta_processor_mode mode, const int field_index, const int bits_written);

uint8_t encode_iterated_body_field(iterated_message *message, int field_index, char *data, char *baseline)
{
    uint8_t changed = 0;
    const _message_definition *definition = message_delta_global_message_list[message->definition_type];
    const _field_reference_definition *field = &definition->body_fields.field_references[field_index]; /* const: points into const definition's inline array, read-only here 2026-07-31 (C4090) */

    char *baseline_value = baseline ? &baseline[field->baseline_offset] : 0;
    int bits = field->properties->encode(field->properties, baseline_value, &data[field->offset],
                                         &message->iteration_data_stream);

    if ( message->mode == _message_delta_mode_incremental )
        changed = (uint8_t)bitstream_write_bit(&message->iteration_field_map_stream, bits != 0);
    else if ( bits > 0 )
        changed = 1;

    message->iteration_body_bits_written += bits;
    last_iteration_field_map[field_index] = bits != 0;
    metrics_record_field_encode(definition, message->mode, field_index, bits);
    return changed;
}
