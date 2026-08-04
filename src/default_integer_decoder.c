/* default_integer_decoder @0x8379A8D8 — decode a field's integer payload according to its width selector
 * (field_properties->parameters, first dword): 0 = byte, 1 = 16-bit, 2 = 32-bit, 3/4/5/6 = 1/3/5/6-bit
 * sub-byte fields (pre-zeroed before the narrow read, since bitstream_read_bits ORs partial bits into the
 * destination rather than fully overwriting it for reads under 32 bits). Any other selector value is a no-op
 * (never observed in practice). Returns the number of bits actually read.
 *
 * DEVIATION: the decompiler modeled the return value as the HIDWORD of a 64-bit local seeded from
 * `(unsigned int)destination_data` — a 64-bit-register-pair artifact of this ABI's calling convention, not a
 * real dependency on destination_data's value. Restored as a plain int accumulator. */

#include "headers/field_properties_definition.h"
#include "headers/field_type_enumeration_parameters.h"
#include "headers/bitstream_t.h"

extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);

int default_integer_decoder(const _field_properties_definition *const field_properties,
    const void *const baseline_data, void *destination_data, bitstream_t *const input_stream)
{
    unsigned int width_selector = *(const unsigned int *)field_properties->parameters;
    int bits_read = 0;

    if ( width_selector > _field_width_6_bits )
        return bits_read;

    switch ( width_selector )
    {
    case _field_width_small:
        bits_read = bitstream_read_bits(input_stream, (unsigned int *)destination_data, 8);
        break;
    case _field_width_medium:
        bits_read = bitstream_read_bits(input_stream, (unsigned int *)destination_data, 0x10);
        break;
    case _field_width_large:
        bits_read = bitstream_read_bits(input_stream, (unsigned int *)destination_data, 0x20);
        break;
    case _field_width_1_bit:
        *(unsigned char *)destination_data = 0;
        bits_read = bitstream_read_bits(input_stream, (unsigned int *)destination_data, 1);
        break;
    case _field_width_3_bits:
        *(unsigned char *)destination_data = 0;
        bits_read = bitstream_read_bits(input_stream, (unsigned int *)destination_data, 3);
        break;
    default: /* _field_width_5_bits or _field_width_6_bits */
        *(unsigned char *)destination_data = 0;
        bits_read = bitstream_read_bits(input_stream, (unsigned int *)destination_data,
            width_selector == _field_width_5_bits ? 5 : 6);
        break;
    }

    return bits_read;
}
