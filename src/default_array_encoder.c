/* default_array_encoder @0x8379AF80 — encode a fixed-size "array" message-delta field: for each element,
 * encode it via the shared element field-type's encoder and write a per-element "changed" bit (one bit
 * per element, at a fixed position right after the array's own overhead bits) recording whether that
 * element's encode call actually produced any bits. If nothing changed across the whole array, rewinds
 * the stream back to the start and returns 0 (letting the caller omit the field entirely); otherwise
 * returns the total bits written including overhead. For a stateless encode (no baseline), every element
 * is unconditionally encoded (with a null baseline) and no changed-bits are written. Sibling of
 * default_array_decoder.c, which shares the same per-element bit-position convention.
 *
 * DEVIATION: disasm confirms `(-bits_written & ~bits_written) >> 31` is the compiler's chosen form for
 * "did the encode call write any bits" as a 0/1 boolean; reconstructed directly as a `!= 0` comparison. */

#include <stdint.h>
#include <stddef.h>
#include "headers/field_properties_definition.h"
#include "headers/field_type_array_parameters.h"
#include "headers/bitstream_t.h"

extern unsigned int bitstream_tell(const bitstream_t *bit_stream);
extern uint8_t bitstream_seek(bitstream_t *const bit_stream, const int offset, const bitstream_seek_origin origin);
extern unsigned int bitstream_write_bit(bitstream_t *bit_stream, const unsigned int value);

int default_array_encoder(const _field_properties_definition *const field_properties, char *baseline_data,
    char *source_data, bitstream_t *const output_stream)
{
    const _field_type_array_parameters *parameters =
        (const _field_type_array_parameters *)field_properties->parameters;
    int bits_written = 0;

    if ( baseline_data )
    {
        unsigned int array_start_bit = bitstream_tell(output_stream);
        bitstream_seek(output_stream, field_properties->overhead_bits, _bitstream_seek_current);

        for ( int i = 0; i < parameters->count; ++i )
        {
            int element_offset = parameters->source_element_size * i;
            char *baseline_element = &baseline_data[element_offset];
            char *source_element = &source_data[element_offset];

            int element_bits = parameters->member_properties->encode(parameters->member_properties,
                baseline_element, source_element, output_stream);
            bits_written += element_bits;

            unsigned int saved_position = bitstream_tell(output_stream);
            bitstream_seek(output_stream, array_start_bit + i, _bitstream_seek_beginning);
            bitstream_write_bit(output_stream, element_bits != 0);
            bitstream_seek(output_stream, saved_position, _bitstream_seek_beginning);
        }

        if ( bits_written > 0 )
            return bits_written + field_properties->overhead_bits;

        bitstream_seek(output_stream, array_start_bit, _bitstream_seek_beginning);
        return bits_written;
    }

    for ( int i = 0; i < parameters->count; ++i )
        bits_written += parameters->member_properties->encode(parameters->member_properties, NULL /* no baseline_element: stateless encode */,
            &source_data[parameters->source_element_size * i], output_stream);

    return bits_written;
}
