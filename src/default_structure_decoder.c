/* default_structure_decoder @0x8379B500 — decode a "structure" (nested field-group) message-delta
 * field: for each nested field reference, if a baseline is given (incremental decode), first peeks
 * whether that field's "changed" bit was set in the message body (a single bit read at a fixed bit
 * position — field index within the group, from the position right after the group's own overhead bits)
 * and only decodes it if so; for a stateless decode, every nested field is unconditionally decoded (with
 * no baseline). Returns the total number of bits consumed (including the group's own overhead_bits, for
 * the incremental path), or 0 if nothing changed. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/field_type_structure_parameters.h"
#include "headers/bitstream_t.h"

extern unsigned int bitstream_tell(const bitstream_t *bit_stream);
extern uint8_t bitstream_seek(bitstream_t *const bit_stream, const int offset, const bitstream_seek_origin origin);
extern unsigned int bitstream_read_bit(bitstream_t *const bit_stream, uint8_t *const value);

int default_structure_decoder(const _field_properties_definition *const field_properties, char *baseline_data,
    char *destination_data, bitstream_t *const input_stream)
{
    const _field_type_structure_parameters *parameters =
        (const _field_type_structure_parameters *)field_properties->parameters;
    int bits_consumed = 0;

    if ( baseline_data )
    {
        unsigned int group_start_bit = bitstream_tell(input_stream);
        bitstream_seek(input_stream, field_properties->overhead_bits, _bitstream_seek_current);

        for ( int i = 0; i < parameters->field_count; ++i )
        {
            const _field_reference_definition *field = &parameters->members_references[i];

            unsigned int saved_position = bitstream_tell(input_stream);
            uint8_t changed = 0;
            bitstream_seek(input_stream, group_start_bit + i, _bitstream_seek_beginning);
            bitstream_read_bit(input_stream, &changed);
            bitstream_seek(input_stream, saved_position, _bitstream_seek_beginning);

            if ( changed )
                bits_consumed += field->properties->decode(field->properties, &baseline_data[field->baseline_offset],
                    &destination_data[field->offset], input_stream);
        }

        if ( bits_consumed > 0 )
            return bits_consumed + field_properties->overhead_bits;

        return 0;
    }

    for ( int i = 0; i < parameters->field_count; ++i )
    {
        const _field_reference_definition *field = &parameters->members_references[i];
        bits_consumed += field->properties->decode(field->properties, nullptr, &destination_data[field->offset],
            input_stream);
    }

    return bits_consumed;
}
