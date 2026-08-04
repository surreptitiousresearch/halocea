/* decode_message_body @0x837A1880 — decode every included body field of a message: reads the 1-bit "any
 * body fields" flag from the stream (returning 0 immediately if absent), then for each field flagged in
 * `field_included`, decodes it via its field-reference's decode callback (baseline read from
 * baseline_offset, destination written at offset) and records decode metrics, accumulating total bits
 * read. Stops and returns 0 immediately if any single field's decode fails (bits_read <= 0); otherwise
 * returns the total bits consumed across all included fields. */

#include <stdint.h>
#include "headers/message_delta_processor_decoding_information.h"

extern _message_definition *message_delta_global_message_list[];
extern int bitstream_has_bits(const bitstream_t *const bit_stream, const unsigned int bit_count);
extern void metrics_record_field_decode(const _message_definition *const message_definition, message_delta_processor_mode mode, const int field_index, const int bits_read);

/* field_included/baseline_data const: disasm 0x837A1880 only loads through r21 (lbzx) and address-computes r31 (no stores); callers pass const header->field_included / const baseline 2026-07-31 (C4090) */
int decode_message_body(message_delta_processor_decoding_information *const decoding_information, const uint8_t *const field_included, const char *baseline_data, char *destination_data)
{
    const _message_definition *message_definition = message_delta_global_message_list[decoding_information->definition_type];
    int total_bits = 0;

    if ( !bitstream_has_bits(decoding_information->input_stream, 1u) )
        return total_bits;

    int field_count = message_definition->body_fields.count;

    for ( int field_index = 0; field_index < field_count; field_index++ )
    {
        if ( !field_included[field_index] )
            continue;

        const _field_reference_definition *field_reference = &message_definition->body_fields.field_references[field_index];

        const char *baseline_field = baseline_data ? &baseline_data[field_reference->baseline_offset] : 0;
        int bits_read = field_reference->properties->decode(field_reference->properties, (void *)baseline_field,
                             &destination_data[field_reference->offset], decoding_information->input_stream);

        metrics_record_field_decode(message_definition, (message_delta_processor_mode)(baseline_data != 0),
            field_index, bits_read);

        if ( bits_read <= 0 )
            return 0;

        total_bits += bits_read;
    }

    return total_bits;
}
