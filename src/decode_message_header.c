/* decode_message_header @0x837A1988 — decode every header field of a message-delta message from the input
 * stream into the destination struct (each field's properties->decode, at its field offset), recording
 * per-field metrics. Returns the total bits consumed, or 0 as soon as any field fails to decode. */

#include "headers/message_definition.h"
#include "headers/message_delta_metrics.h"
#include "headers/bitstream_t.h"
#include "headers/iterated_message.h"

extern _message_definition **message_delta_global_message_list;

extern void metrics_record_header_field_decode(const _message_definition *const message_definition, const message_delta_processor_mode mode, const int field_index, const int bits_read);

extern int decode_header_field(message_delta_processor_message_definition_type definition_type, int field_index, char *destination_data, bitstream_t *const input_stream);
int decode_message_header(const message_delta_processor_message_definition_type definition_type,
        const message_delta_processor_mode mode, bitstream_t *const input_stream,
        void *const destination_data)
{
    _message_definition *definition = message_delta_global_message_list[definition_type];
    int total_bits = 0;

    for ( int field_index = 0; field_index < definition->header_fields->count; field_index++ )
    {
        /* DEVIATION: verbatim inlined copy of decode_header_field@0x837A0E60 (zero-xref donor) collapsed to a call; field/properties/offset walk (16-byte _field_reference_definition stride, properties@+0/offset@+4) confirmed field-for-field vs donor at disasm grade; definition_type/field_index/destination_data/input_stream pass straight through, no constants to fold */
        int bits = decode_header_field(definition_type, field_index, (char *)destination_data, input_stream);
        metrics_record_header_field_decode(definition, mode, field_index, bits);
        if ( bits <= 0 )
            return 0;
        total_bits += bits;
    }
    return total_bits;
}
