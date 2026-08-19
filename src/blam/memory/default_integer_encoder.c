/* default_integer_encoder @0x8379A798 — default network field encoder for integer-typed fields: reads
 * the field's bit-width class (0-6) from field_properties->parameters[0], skips the write entirely if a
 * baseline is given and matches the source value at that width, otherwise writes source_data to
 * output_stream at the fixed bit width for that class (class 0 = 8 bits; 1 = 16; 2 = 32; 3 = 1; 4 = 3;
 * 5 = 5; default/6 = 6 bits). Returns the number of bits written, or 0 if nothing was written (baseline
 * matched, or the class was out of range). */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/field_type_integer_parameters.h"
#include "headers/bitstream_t.h"

extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

unsigned int default_integer_encoder(const _field_properties_definition *const field_properties,
    const void *const baseline_data, const void *const source_data, bitstream_t *const output_stream)
{
    const _field_type_integer_parameters *parameters =
        (const _field_type_integer_parameters *)field_properties->parameters;
    _field_width bit_width_class = parameters->width;

    if ( (unsigned int)bit_width_class > 6 )
        return 0;

    switch ( bit_width_class )
    {
        case _field_width_small:
            if ( !baseline_data || *(const uint8_t *)baseline_data != *(const uint8_t *)source_data )
                return bitstream_write_bits(output_stream, source_data, 8u);
            break;
        case _field_width_medium:
            if ( !baseline_data || *(const uint16_t *)baseline_data != *(const uint16_t *)source_data )
                return bitstream_write_bits(output_stream, source_data, 0x10u);
            break;
        case _field_width_large:
            if ( !baseline_data || *(const unsigned int *)baseline_data != *(const unsigned int *)source_data )
                return bitstream_write_bits(output_stream, source_data, 0x20u);
            break;
        case _field_width_1_bit:
            if ( !baseline_data || *(const uint8_t *)baseline_data != *(const uint8_t *)source_data )
                return bitstream_write_bits(output_stream, source_data, 1u);
            break;
        case _field_width_3_bits:
            if ( !baseline_data || *(const uint8_t *)baseline_data != *(const uint8_t *)source_data )
                return bitstream_write_bits(output_stream, source_data, 3u);
            break;
        case _field_width_5_bits:
            if ( !baseline_data || *(const uint8_t *)baseline_data != *(const uint8_t *)source_data )
                return bitstream_write_bits(output_stream, source_data, 5u);
            break;
        default:   /* _field_width_6_bits */
            if ( !baseline_data || *(const uint8_t *)baseline_data != *(const uint8_t *)source_data )
                return bitstream_write_bits(output_stream, source_data, 6u);
            break;
    }

    return 0;
}
