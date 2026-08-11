/* decode_body_field @0x837A0DD0 — decodes one message body field via its field-reference's decode callback
 * (at offset +84 within the field reference), and records decode metrics. Mirrors decode_header_field.c's
 * indexing but reads a field_reference array directly rather than through a header_fields block. */

#include "headers/message_definition.h"
#include "headers/bitstream_t.h"
#include "headers/message_delta_processor_mode.h"
#include <stddef.h>

extern _message_definition *const message_delta_global_message_list[47];
extern void metrics_record_field_decode(const _message_definition *const message_definition, message_delta_processor_mode mode, const int field_index, const int bits_read);

int decode_body_field(const message_delta_processor_message_definition_type definition_type, int field_index,
    const void *const baseline_data, void *const destination_data, bitstream_t *const input_stream)
{
    _message_definition *message_definition = message_delta_global_message_list[definition_type];
    _field_reference_definition *field_reference = &message_definition->body_fields.field_references[field_index];

    const char *baseline_field = baseline_data ? &((const char *)baseline_data)[field_reference->offset] : NULL;

    int bits_read = field_reference->properties->decode(field_reference->properties, (void *)baseline_field,
        &((char *)destination_data)[field_reference->offset], input_stream);

    metrics_record_field_decode(message_definition, (message_delta_processor_mode)(baseline_data != NULL),
        field_index, bits_read);

    return bits_read;
}
