/* decode_message_dependent_header_iterated @0x837A21A0 — decode the per-iteration "dependent" header of an
 * iterated message-delta message. First it establishes the field-inclusion map (header->field_included):
 * in incremental mode it reads one presence bit per body field from the stream (stopping at the first 0),
 * in stateless mode it checks the stream has room for the whole header field-reference set and marks every
 * body field included. It then, if the map was established and the message has header fields, decodes each
 * header field via its field-type decode callback (properties->decode at +84, writing into custom_header at
 * the field's offset) and accumulates the bits. The unused tail of the field map is zeroed and the map is
 * snapshotted into last_iteration_field_map. Returns the accumulated bit count, or 0 on failure.
 *
 * Sibling of decode_message_header (@0x837A1988); reuses the same field_references[i].properties->decode
 * dispatch. The decompiler's `(char *)header_fields + v20` byte-stepping is the field_references[] array walk
 * (field_references is at set+8, properties at field+0, offset at field+4 — so *(v21+8)/*(v21+12) are
 * field->properties/field->offset). The metrics_record_* calls are no-ops in this build. */

#include <stdint.h>
#include "headers/message_delta_processor_header.h"
#include "headers/iteration_dependent_overhead_type.h"
#include <string.h>
#include "headers/blam_data_globals.h"

extern _message_definition **message_delta_global_message_list;

extern uint8_t bitstream_has_bits(const bitstream_t *const bit_stream, const unsigned int bit_count);
extern unsigned int bitstream_read_bit(bitstream_t *const bit_stream, uint8_t *const value);
extern void metrics_record_iteration_dependent_overhead_decode(const _message_definition *const message_definition, const message_delta_processor_mode mode, const iteration_dependent_overhead_type type, const int bits);
extern void metrics_record_header_field_decode(const _message_definition *const message_definition, const message_delta_processor_mode mode, const int field_index, const int bits_read);

extern int decode_header_field(message_delta_processor_message_definition_type definition_type, int field_index, char *destination_data, bitstream_t *const input_stream);
int decode_message_dependent_header_iterated(message_delta_processor_header *const header)
{
    message_delta_processor_decoding_information *decoding_information = header->decoding_information;
    int total_bits = 0;
    _message_definition *definition = message_delta_global_message_list[header->decoding_information->definition_type];
    signed int field_count = definition->body_fields.count;

    int proceed;
    uint8_t established;

    if ( header->decoding_information->mode )
    {
        uint8_t has_bits = bitstream_has_bits(decoding_information->input_stream,
                                                      definition->header_size_in_bits);
        proceed = has_bits;
        established = has_bits;
        if ( has_bits )
        {
            for ( int i = 0; i < field_count; ++total_bits )
            {
                unsigned int bit = bitstream_read_bit(decoding_information->input_stream, &header->field_included[i]);
                established = (bit == 1);
                proceed = (bit == 1);
                if ( bit != 1 )
                    break;
                ++i;
            }
        }
    }
    else
    {
        _message_definition_field_reference_set *header_fields = definition->header_fields;
        if ( header_fields->count <= 0 )
            established = 1;
        else
            established = bitstream_has_bits(decoding_information->input_stream, header_fields->maximum_data_size);
        proceed = established;
        if ( established )
            memset(header->field_included, 1, field_count);
    }

    metrics_record_iteration_dependent_overhead_decode(definition, header->decoding_information->mode,
                                                       FIRST_ITERATION_DEPENDENT_OVERHEAD_TYPE, total_bits);

    if ( proceed )
    {
        message_delta_processor_message_definition_type definition_type = decoding_information->definition_type;
        if ( message_delta_global_message_list[definition_type]->header_fields->count > 0 )
        {
            void *custom_header = header->custom_header;
            bitstream_t *input_stream = decoding_information->input_stream;
            int header_field_bits = 0;
            message_delta_processor_mode mode = decoding_information->mode;
            int field_index = 0;
            _message_definition *header_definition = message_delta_global_message_list[definition_type];
            if ( header_definition->header_fields->count > 0 )
            {
                while ( 1 )
                {
                    /* DEVIATION: verbatim inlined copy of decode_header_field@0x837A0E60 (zero-xref donor) collapsed to a call; field/properties/offset walk (16-byte _field_reference_definition stride, properties@+0/offset@+4) confirmed field-for-field vs donor at disasm grade; definition_type/field_index/custom_header/input_stream pass straight through, no constants to fold */
                    int bits = decode_header_field(definition_type, field_index, (char *)custom_header, input_stream);
                    metrics_record_header_field_decode(header_definition, mode, field_index, bits);
                    if ( bits <= 0 )
                        break;
                    ++field_index;
                    header_field_bits += bits;
                    if ( field_index >= header_definition->header_fields->count )
                    {
                        if ( header_field_bits <= 0 )
                            break;
                        total_bits += header_field_bits;
                        established = 1;
                        goto done;
                    }
                }
            }
            established = 0;
        }
    }

done:
    memset(&header->field_included[field_count], 0, 64 - field_count);
    memcpy(last_iteration_field_map, header->field_included, sizeof(last_iteration_field_map));
    return established == 0 ? 0 : total_bits;
}
