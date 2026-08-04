/* default_array_decoder @0x8379B0E8 — decode a fixed-size "array" message-delta field: for each
 * element, if a baseline is given (incremental decode), peeks whether that element's "changed" bit was
 * set (one bit per element, read at a fixed position right after the array's own overhead bits) — if
 * changed, decodes it via the shared element field-type's decoder; if unchanged, just copies the
 * baseline element straight through. For a stateless decode, every element is unconditionally decoded
 * (with no baseline). Returns the total number of bits consumed (including the array's own
 * overhead_bits, for the incremental path), or 0 if nothing changed. Sibling of
 * default_structure_decoder.c, which shares the same "changed" bit-peeking shape for per-field-reference
 * (rather than per-element) groups. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/field_type_array_parameters.h"
#include "headers/bitstream_t.h"

extern void *memcpy(void *destination, const void *source, unsigned int size);
extern unsigned int bitstream_tell(const bitstream_t *bit_stream);
extern uint8_t bitstream_seek(bitstream_t *const bit_stream, const int offset, const bitstream_seek_origin origin);
extern unsigned int bitstream_read_bit(bitstream_t *const bit_stream, uint8_t *const value);

int default_array_decoder(const _field_properties_definition *const field_properties, char *baseline_data,
    char *destination_data, bitstream_t *const input_stream)
{
    const _field_type_array_parameters *parameters =
        (const _field_type_array_parameters *)field_properties->parameters;
    int bits_consumed = 0;

    if ( baseline_data )
    {
        unsigned int array_start_bit = bitstream_tell(input_stream);
        bitstream_seek(input_stream, field_properties->overhead_bits, _bitstream_seek_current);

        for ( int i = 0; i < parameters->count; ++i )
        {
            int element_offset = parameters->source_element_size * i;
            char *baseline_element = &baseline_data[element_offset];
            char *destination_element = &destination_data[element_offset];

            unsigned int saved_position = bitstream_tell(input_stream);
            unsigned __int8 changed = 0;
            bitstream_seek(input_stream, array_start_bit + i, _bitstream_seek_beginning);
            bitstream_read_bit(input_stream, &changed);
            bitstream_seek(input_stream, saved_position, _bitstream_seek_beginning);

            if ( changed )
                bits_consumed += parameters->member_properties->decode(parameters->member_properties,
                    baseline_element, destination_element, input_stream);
            else
                memcpy(destination_element, baseline_element, parameters->source_element_size);
        }

        if ( bits_consumed > 0 )
            return bits_consumed + field_properties->overhead_bits;

        return 0;
    }

    for ( int i = 0; i < parameters->count; ++i )
        bits_consumed += parameters->member_properties->decode(parameters->member_properties, nullptr,
            &destination_data[parameters->source_element_size * i], input_stream);

    return bits_consumed;
}
