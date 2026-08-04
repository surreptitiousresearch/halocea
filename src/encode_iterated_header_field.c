/* encode_iterated_header_field @0x837A0C80 — encode one header field of the current message-delta iteration
 * by invoking the field type's encode callback over the field's value, writing into the iteration data
 * stream. Returns whether any bits were written; on success, tallies the bits into
 * iteration_header_bits_written. Mirrors encode_iterated_body_field.c's header-field counterpart. */

#include "headers/iterated_message.h"
#include "headers/message_definition_field_reference_set.h"

extern _message_definition *message_delta_global_message_list[];
extern void metrics_record_header_field_encode(const _message_definition *definition, message_delta_processor_mode mode, int field_index, int bits);

int encode_iterated_header_field(iterated_message *const message, const int field_index, const void *const data)
{
    const _message_definition *definition = message_delta_global_message_list[message->definition_type];
    const _field_reference_definition *field = &definition->header_fields->field_references[field_index];

    int bits = field->properties->encode(field->properties, 0,
                                         (char *)data + field->offset, &message->iteration_data_stream);

    int changed = bits > 0;
    if ( changed )
        message->iteration_header_bits_written += bits;

    metrics_record_header_field_encode(definition, message->mode, field_index, bits);
    return changed;
}
